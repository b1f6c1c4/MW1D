using System.Collections.Generic;
using System.Threading;

namespace MWScheduler
{
    public class Scheduler<T>
    {
        public delegate void PopEventHandler(T obj);

        public delegate void WorkEventHandler(T obj);

        public event PopEventHandler OnPop;
        public event WorkEventHandler OnWork;

        private readonly object m_Lock = new object();

        private readonly List<Thread> m_Threads;

        public IInfQueue<T> TheQueue { get; set; }

        public Scheduler(int n)
        {
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
                T obj;
                lock (m_Lock)
                {
                    obj = TheQueue.Pop();
                    OnPop?.Invoke(obj);
                }
                OnWork?.Invoke(obj);
            }
        }
    }
}
