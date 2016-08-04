using System;
using System.Collections.Generic;

namespace MWScheduler
{
    public class LambdaComparer<T, TOut> : IComparer<T>
    {
        private readonly Func<T, TOut> m_Lambda;
        private readonly IComparer<TOut> m_BaseComparer;

        public LambdaComparer(Func<T, TOut> lambda, IComparer<TOut> baseComparer)
        {
            m_Lambda = lambda;
            m_BaseComparer = baseComparer;
        }

        public int Compare(T x, T y) => m_BaseComparer.Compare(m_Lambda(x), m_Lambda(y));
    }
}
