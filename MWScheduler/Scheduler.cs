using System.Collections.Generic;
using System.Threading;

namespace MWScheduler
{
    public class Scheduler<T>
        where T : class
    {
        public delegate void EventHandler(T obj);

        public event EventHandler OnLock;
        public event EventHandler OnPop;
        public event EventHandler OnWork;

        private readonly List<Thread> m_Threads;

        private readonly IInfQueue<T> m_TheQueue;

        public Scheduler(int n, IInfQueue<T> theQueue)
        {
            m_TheQueue = theQueue;
            m_Threads = new List<Thread>();
            for (var i = 0; i < n; i++)
                m_Threads.Add(new Thread(WorkerThreadEntryPoint));
        }

        public void Start()
        {
            foreach (var thread in m_Threads)
                thread.Start();
        }

        public void WaitAll()
        {
            foreach (var thread in m_Threads)
                thread.Join();
        }

        private void WorkerThreadEntryPoint()
        {
            while (true)
            {
                var obj = m_TheQueue.Lock();
                OnLock?.Invoke(obj);
                OnWork?.Invoke(obj);
                if (m_TheQueue.Pop(obj))
                    OnPop?.Invoke(obj);
            }
        }
    }
}
