using System.Collections.Generic;
using System.Linq;

namespace MWScheduler
{
    public class MergedInfQueue<T> : List<IInfQueue<T>>, IInfQueue<T>
        where T : class
    {
        private readonly IComparer<IInfQueue<T>> m_Comparer;

        public MergedInfQueue(IComparer<T> comparer)
        {
            var nullComparer = new NullComparer<T>(comparer, false);
            m_Comparer = new LambdaComparer<IInfQueue<T>, T>(q => q.Top, nullComparer);
        }

        protected IInfQueue<T> Least
        {
            get
            {
                Sort(m_Comparer);
                return this.First();
            }
        }

        public T Top => Least.Top;

        public T Lock() => Least.Lock();

        public virtual bool Pop(T obj) => this.Any(queue => queue.Pop(obj));

        public override string ToString() => $"{Top?.ToString() ?? "LOCKED"} ...";
    }
}
