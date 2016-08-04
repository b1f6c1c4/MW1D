using System.Collections.Generic;

namespace MWScheduler
{
    public class NullComparer<T> : IComparer<T>
        where T : class
    {
        private readonly IComparer<T> m_BaseComparer;

        private readonly bool m_NullLess;

        public NullComparer(IComparer<T> baseComparer, bool nullLess = true)
        {
            m_BaseComparer = baseComparer;
            m_NullLess = nullLess;
        }

        public int Compare(T x, T y)
        {
            if (x == null &&
                y == null)
                return 0;
            if (x == null)
                return m_NullLess ? -1 : +1;
            if (y == null)
                return m_NullLess ? +1 : -1;
            return m_BaseComparer.Compare(x, y);
        }
    }
}
