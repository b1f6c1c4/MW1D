using System.Collections.Generic;

namespace MWScheduler
{
    public class ProbBundle : MergedInfQueue<Config>
    {
        public ProbBundle(IComparer<IInfQueue<Config>> comparer, IEnumerable<Rational> probs, string strategy,
                          bool mirror = false)
            : base(comparer)
        {
            foreach (var prob in probs)
                AddQueue(prob, strategy, mirror);
        }

        private void AddQueue(Rational prob, string strategy, bool mirror = false)
        {
            Add(
                new ConstProbInfQueue
                    {
                        Width = 2,
                        Probability = prob,
                        Strategy = strategy
                    });
            if (mirror && prob.Denominator != prob.Numerator * 2)
                AddQueue(new Rational(prob.Denominator - prob.Numerator, prob.Denominator), strategy);
        }
    }
}
