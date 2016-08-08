#include "stdafx.h"
#include "FLHeuristic.h"
#include <memory>

FLHeuristic::~FLHeuristic() { }

void FLHeuristic::SetBase(std::shared_ptr<FLHeuristic> base)
{
    m_Base = base;
}

std::string FLHeuristic::GetDescription() const
{
    auto &&me = GetOneDescription();
    return m_Base == nullptr ? me : m_Base->GetDescription() + me;
}

void FLHeuristic::Process(const ExtendedMacro &macro, std::vector<size_t> &target)
{
    if (m_Base != nullptr)
        m_Base->Process(macro, target);

    if (target.size() <= 1)
        return;

    ProcessOne(macro, target);
}

FLHeuristic::FLHeuristic() { }

std::string FLP::GetOneDescription() const
{
    return "P";
}

prob FLP::GetValue(const ExtendedMacro &macro, std::vector<size_t> &target, size_t id)
{
    prob pb = 0;
    for (auto it = macro.begin(); it != macro.end(); ++it)
        if ((*it)[id])
            pb += it->GetProb();
    return pb;
}

prob FLDist::GetValue(const ExtendedMacro &macro, std::vector<size_t> &target, size_t id)
{
    std::vector<std::shared_ptr<ExtendedMacro>> dist;

    block_t lst[] = {0, 1, 2};
    for (auto m : lst)
        dist.push_back(std::make_shared<ExtendedMacro>(macro, id, m));

    return GetValue(dist, target, id);
}

std::string FLU::GetOneDescription() const
{
    return "U";
}

prob FLU::GetValue(const std::vector<std::shared_ptr<ExtendedMacro>> &dist, std::vector<size_t> &target, size_t id)
{
    prob p = 0;
    for (auto macro : dist)
#ifdef USE_CAS
        if (!macro->GetTotalProb()->ZeroQ())
#else
        if (macro->GetTotalProb() != 0)
#endif
        {
            auto n = macro->GetN();

            std::vector<prob> probs(n, 0);
            for (auto it = macro->begin(); it != macro->end(); ++it)
                for (size_t i = 0; i < n; i++)
                    if (!macro->IsOpen(i))
                        if ((*it)[i])
                            probs[i] += it->GetProb();

            auto pMin = macro->GetTotalProb();
            for (size_t i = 0; i < n; i++)
                if (!macro->IsOpen(i))
                    if (probs[i] < pMin)
                        pMin = probs[i];

            p += macro->GetTotalProb() * pMin;
        }
    return p;
}

prob FLSE::GetValue(const std::vector<std::shared_ptr<ExtendedMacro>> &dist, std::vector<size_t> &target, size_t id)
{
    std::vector<prob> safeDist;
    for (auto macro : dist)
#ifdef USE_CAS
        if (!macro->GetTotalProb()->ZeroQ())
#else
        if (macro->GetTotalProb() != 0)
#endif
        {
            auto n = macro->GetN();

            bits possible(n, false);
            for (auto it = macro->begin(); it != macro->end(); ++it)
                for (size_t i = 0; i < n; i++)
                    if ((*it)[i])
                        possible[i] = true;

            size_t cnt = 0;
            for (size_t i = 0; i < n; i++)
                if (!possible[i] && !macro->IsOpen(i))
                    cnt++;

            if (safeDist.size() <= cnt)
                safeDist.resize(cnt + 1, 0);
            safeDist[cnt] += macro->GetTotalProb();
        }
    return GetValue(safeDist);
}

std::string FLS::GetOneDescription() const
{
    return "S";
}

prob FLS::GetValue(const std::vector<prob> &safeDist)
{
    prob p = 0;
    for (size_t i = 1; i < safeDist.size(); i++)
        p -= safeDist[i];
    return p;
}

std::string FLE::GetOneDescription() const
{
    return "E";
}

prob FLE::GetValue(const std::vector<prob> &safeDist)
{
    prob p = 0;
    for (size_t i = 1; i < safeDist.size(); i++)
        p -= safeDist[i] * i;
    return p;
}

std::string FLQ::GetOneDescription() const
{
    return "Q";
}

prob FLQ::GetValue(const std::vector<std::shared_ptr<ExtendedMacro>> &dist, std::vector<size_t> &target, size_t id)
{
    prob p = 0;
    for (auto macro : dist)
#ifdef USE_CAS
        if (!macro->GetTotalProb()->ZeroQ())
#else
        if (macro->GetTotalProb() != 0)
#endif
            p -= macro->GetTotalProb() * Log(macro->GetTotalProb());
    return p;
}

prob FLQ::Log(prob value)
{
#ifdef USE_CAS
    auto exp = std::make_shared<MergedExpression>("Log");
    exp->push_back(value);
    return std::shared_ptr<IExpression>(exp);
#else
    return log(value);
#endif
}

std::string FLZ::GetOneDescription() const
{
    return "Z";
}

prob FLZ::GetValue(const std::vector<std::shared_ptr<ExtendedMacro>> &dist, std::vector<size_t> &target, size_t id)
{
    return 0 - dist[0]->GetTotalProb();
}

std::shared_ptr<FLHeuristic> CreateHeuristic(const std::string &str)
{
    std::shared_ptr<FLHeuristic> ptr = nullptr;
    for (auto ch : str)
    {
        std::shared_ptr<FLHeuristic> tmp;
        switch (ch)
        {
        case 'P':
        case 'p':
            tmp = std::make_shared<FLP>();
            break;
        case 'U':
        case 'u':
            tmp = std::make_shared<FLU>();
            break;
        case 'S':
        case 's':
            tmp = std::make_shared<FLS>();
            break;
        case 'E':
        case 'e':
            tmp = std::make_shared<FLE>();
            break;
        case 'Q':
        case 'q':
            tmp = std::make_shared<FLQ>();
            break;
        case 'Z':
        case 'z':
            tmp = std::make_shared<FLZ>();
            break;
        default:
            continue;
        }
        tmp->SetBase(ptr);
        ptr = tmp;
    }
    return ptr;
}
