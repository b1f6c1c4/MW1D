namespace MWScheduler
{
    public class MineBundle : MergedInfQueue<Config>
    {
        private ConstMineInfQueue m_CriticalQueue;

        public override void Pop()
        {
            base.Pop();
            if (Least != m_CriticalQueue)
                return;

            var queue = new ConstMineInfQueue
                            {
                                Width = m_CriticalQueue.Width + 1,
                                IncreaseMines = !m_CriticalQueue.IncreaseMines,
                                Strategy = m_CriticalQueue.Strategy,
                                TotalMines = m_CriticalQueue.TotalMines,
                                Prev1T = m_CriticalQueue.Top.Elapsed.Ticks,
                                Prev2T = null
                            };
            if (!m_CriticalQueue.IncreaseMines)
                queue.TotalMines++;

            AddQueue(queue);
            m_CriticalQueue = queue;
        }
    }
}
