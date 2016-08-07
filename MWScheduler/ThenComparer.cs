using System.Collections.Generic;

namespace MWScheduler
{
    public class ThenComparer<T> : IComparer<T>
    {
        private readonly IComparer<T> m_BaseComparer1;
        private readonly IComparer<T> m_BaseComparer2;

        public ThenComparer(IComparer<T> baseComparer1, IComparer<T> baseComparer2)
        {
            m_BaseComparer1 = baseComparer1;
            m_BaseComparer2 = baseComparer2;
        }

        public int Compare(T x, T y)
        {
            var res = m_BaseComparer1.Compare(x, y);
            return res == 0 ? m_BaseComparer2.Compare(x, y) : res;
        }
    }
}
