using System.Collections.Generic;

namespace MWScheduler
{
    public class BoundedComparer : IComparer<Config>
    {
        private readonly double m_Bound;

        public BoundedComparer(double bound) { m_Bound = bound; }

        public int Compare(Config x, Config y)
        {
            if (x.Width >= m_Bound * y.Width)
                return 1;
            if (y.Width >= m_Bound * x.Width)
                return -1;

            return x.Elapsed.CompareTo(y.Elapsed);
        }
    }
}
