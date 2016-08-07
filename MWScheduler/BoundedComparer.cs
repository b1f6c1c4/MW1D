using System.Collections.Generic;

namespace MWScheduler
{
    public class BoundedComparer : IComparer<double>
    {
        private readonly double m_Bound;

        public BoundedComparer(double bound) { m_Bound = bound; }

        public int Compare(double x, double y)
        {
            if (x >= m_Bound * y)
                return 1;
            if (y >= m_Bound * x)
                return -1;

            return 0;
        }
    }
}
