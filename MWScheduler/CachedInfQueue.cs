using System.IO;

namespace MWScheduler
{
    public class CachedInfQueue<T> : IInfQueue<T>
        where T : class, ICacheable
    {
        public delegate void EventHandler(string filePath, T obj);

        public event EventHandler OnSkipping;
        public event EventHandler OnPop;

        public string BaseDir { get; set; }

        private readonly IInfQueue<T> m_BaseQueue;

        public CachedInfQueue(IInfQueue<T> baseQueue) { m_BaseQueue = baseQueue; }

        public T Top
        {
            get
            {
                Check();
                return m_BaseQueue.Top;
            }
        }

        public T Lock()
        {
            Check();
            return m_BaseQueue.Lock();
        }

        public bool Pop(T obj)
        {
            Check();
            var res = m_BaseQueue.Pop(obj);
            if (res)
                OnPop?.Invoke(Path.Combine(BaseDir, obj.FileName), obj);
            return res;
        }

        private string TopFileName =>
            m_BaseQueue.Top != null ? Path.Combine(BaseDir, m_BaseQueue.Top.FileName) : null;

        private void Check()
        {
            if (TopFileName == null)
                return;

            while (File.Exists(TopFileName))
            {
                m_BaseQueue.Lock();
                OnSkipping?.Invoke(TopFileName, m_BaseQueue.Top);
                m_BaseQueue.Pop(m_BaseQueue.Top);
            }
        }
    }
}
