using System.Threading;

namespace MWScheduler
{
    public class SynInfQueue<T> : IInfQueue<T>
        where T : class
    {
        public delegate void EventHandler();

        public event EventHandler OnWaiting;
        public event EventHandler OnWaited;

        private readonly object m_Lock = new object();

        private readonly IInfQueue<T> m_BaseQueue;

        public SynInfQueue(IInfQueue<T> baseQueue) { m_BaseQueue = baseQueue; }

        public T Top
        {
            get
            {
                lock (m_Lock)
                    return m_BaseQueue.Top;
            }
        }

        public bool IsLocked
        {
            get
            {
                lock (m_Lock)
                    return m_BaseQueue.IsLocked;
            }
        }

        public T Lock()
        {
            lock (m_Lock)
            {
                if (m_BaseQueue.IsLocked)
                {
                    OnWaiting?.Invoke();
                    while (m_BaseQueue.IsLocked)
                        Monitor.Wait(m_Lock);
                    OnWaited?.Invoke();
                }
                return m_BaseQueue.Lock();
            }
        }

        public bool Pop(T obj)
        {
            lock (m_Lock)
            {
                var res = m_BaseQueue.Pop(obj);
                if (res)
                    Monitor.PulseAll(m_Lock); // number of queues may increase
                return res;
            }
        }
    }
}
