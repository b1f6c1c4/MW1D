using System.Collections.Generic;
using System.Linq;

namespace MWScheduler
{
    public class MergedInfQueue<T> : List<IInfQueue<T>>, IInfQueue<T>
    {
        private readonly IComparer<T> m_Comparer;

        public MergedInfQueue(IComparer<T> comparer) { m_Comparer = comparer; }

        protected IInfQueue<T> Least
        {
            get
            {
                Sort((q1, q2) => m_Comparer.Compare(q1.Top, q2.Top));
                return this.First();
            }
        }

        public T Top => Least.Top;

        public virtual T Pop() => Least.Pop();
    }
}
