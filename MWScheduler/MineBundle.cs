using System.Collections.Generic;

namespace MWScheduler
{
    public class MineBundle : MergedInfQueue<Config>
    {
        private ConstMineInfQueue m_CriticalQueue;

        public MineBundle(IComparer<Config> comparer, int width, int mines, string strategy) : base(comparer)
        {
            var queue = new ConstMineInfQueue
                            {
                                Width = width,
                                IncreaseMines = true,
                                TotalMines = mines,
                                Strategy = strategy
                            };

            Add(queue);
            m_CriticalQueue = queue;
        }

        public override bool Pop(Config obj)
        {
            if (!base.Pop(obj))
                return false;

            if (Least != m_CriticalQueue)
                return true;

            var queue = new ConstMineInfQueue
                            {
                                Width = m_CriticalQueue.Width + 1,
                                IncreaseMines = !m_CriticalQueue.IncreaseMines,
                                TotalMines = m_CriticalQueue.TotalMines,
                                Strategy = m_CriticalQueue.Strategy,
                                Prev1T = m_CriticalQueue.Top.Elapsed.Ticks,
                                Prev2T = null
                            };
            if (!m_CriticalQueue.IncreaseMines)
                queue.TotalMines++;

            Add(queue);
            m_CriticalQueue = queue;
            return true;
        }
    }
}
