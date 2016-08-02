using System.Collections.Generic;
using System.Linq;

namespace MWScheduler
{
    public class MergedInfQueue<T> : IInfQueue<T>
    {
        private readonly List<IInfQueue<T>> m_BaseQueues = new List<IInfQueue<T>>();

        public IComparer<T> Comparer { get; set; }

        public void AddQueue(IInfQueue<T> queue) => m_BaseQueues.Add(queue);

        protected IInfQueue<T> Least
        {
            get
            {
                m_BaseQueues.Sort((q1, q2) => Comparer.Compare(q1.Top, q2.Top));
                return m_BaseQueues.First();
            }
        }

        public T Top => Least.Top;

        public virtual void Pop() => Least.Pop();
    }
}
