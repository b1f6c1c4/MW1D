using System.Collections.Generic;
using System.Linq;

namespace MWScheduler
{
    public class MergedInfQueue<T> : List<IInfQueue<T>>, IInfQueue<T>
        where T : class
    {
        private readonly IComparer<IInfQueue<T>> m_Comparer;

        public MergedInfQueue(IComparer<IInfQueue<T>> comparer) { m_Comparer = comparer; }

        protected IInfQueue<T> Least
        {
            get
            {
                Sort(m_Comparer);
                return this.First();
            }
        }

        public T Top => Least.Top;

        public bool IsLocked => Least.IsLocked;

        public T Lock() => Least.Lock();

        public virtual bool Pop(T obj) => this.Any(queue => queue.Pop(obj));

        public override string ToString() => $"{Top?.ToString() ?? "LOCKED"} ...";
    }
}
