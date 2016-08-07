using System.Collections.Generic;

namespace MWScheduler
{
    public class BooleanComparer : IComparer<bool>
    {
        private readonly bool m_FalseLess;

        public BooleanComparer(bool falseLess = true) { m_FalseLess = falseLess; }

        public int Compare(bool x, bool y)
        {
            if (x && y)
                return 0;
            if (y)
                return m_FalseLess ? -1 : +1;
            if (x)
                return m_FalseLess ? +1 : -1;
            return 0;
        }
    }
}
