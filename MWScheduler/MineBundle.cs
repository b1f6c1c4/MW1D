using System;
using System.Collections.Generic;

namespace MWScheduler
{
    public class MineBundle : MergedInfQueue<Config>
    {
        private ConstMineInfQueue m_CriticalQueue;

        public MineBundle(IComparer<IInfQueue<Config>> comparer, int width, int mines, string strategy)
            : base(comparer)
        {
            var queue = new ConstMineInfQueue
                            {
                                Width = width,
                                IncreaseMines = false,
                                TotalMines = mines,
                                Strategy = strategy
                            };

            Add(queue);
            m_CriticalQueue = queue;
        }

        public override bool Pop(Config obj)
        {
            if (!obj.Equals(m_CriticalQueue.Top))
                return base.Pop(obj);

            if (!base.Pop(obj))
                throw new ApplicationException();

            var queue = new ConstMineInfQueue
                            {
                                Width = m_CriticalQueue.Top.Width,
                                IncreaseMines = !m_CriticalQueue.IncreaseMines,
                                TotalMines = m_CriticalQueue.Top.TotalMines,
                                Strategy = m_CriticalQueue.Strategy,
                                Prev1T = m_CriticalQueue.Top.Elapsed,
                                Prev2T = null
                            };
            if (!m_CriticalQueue.IncreaseMines)
                queue.TotalMines++;
            else
                queue.TotalMines--;

            Add(queue);

            m_CriticalQueue = queue;
            return true;
        }
    }
}
