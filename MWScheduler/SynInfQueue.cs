namespace MWScheduler
{
    public class SynInfQueue<T> : IInfQueue<T>
        where T : class
    {
        private readonly object m_Lock = new object();

        private readonly IInfQueue<T> m_BaseQueue;

        public SynInfQueue(IInfQueue<T> baseQueue) { m_BaseQueue = baseQueue; }

        public static SynInfQueue<T> Syncronize(IInfQueue<T> queue) => new SynInfQueue<T>(queue);

        public T Top
        {
            get
            {
                lock (m_Lock)
                    return m_BaseQueue.Top;
            }
        }

        public T Lock()
        {
            lock (m_Lock)
                return m_BaseQueue.Lock();
        }

        public bool Pop(T obj)
        {
            lock (m_Lock)
                return m_BaseQueue.Pop(obj);
        }
    }
}
