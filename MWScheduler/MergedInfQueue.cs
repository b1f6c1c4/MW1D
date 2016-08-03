using System.Collections.Generic;
using System.Linq;

namespace MWScheduler
{
    public class MergedInfQueue<T> : List<IInfQueue<T>>, IInfQueue<T>
        where T : class
    {
        private readonly IComparer<T> m_Comparer;

        public MergedInfQueue(IComparer<T> comparer) { m_Comparer = comparer; }

        protected IInfQueue<T> Least
        {
            get
            {
                Sort(
                     (q1, q2) =>
                     {
                         if (q1.Top == null &&
                             q2.Top == null)
                             return 0;
                         if (q1.Top == null)
                             return 1;
                         if (q2.Top == null)
                             return -1;
                         return m_Comparer.Compare(q1.Top, q2.Top);
                     });
                return this.First();
            }
        }

        public T Top => Least.Top;

        public T Lock() => Least.Lock();

        public virtual bool Pop(T obj) => this.Any(queue => queue.Pop(obj));
    }
}
