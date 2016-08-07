using System;
using System.Threading;

namespace MWScheduler
{
    public abstract class ConfigInfQueue : IInfQueue<Config>
    {
        private Config m_Top;

        public Config Top => m_Top ?? (m_Top = GenerateOne());

        public bool IsLocked { get; private set; }

        public long? Prev1T { get; set; }
        public long? Prev2T { get; set; }
        public int Width { get; set; }
        public string Strategy { get; set; }

        public Config Lock()
        {
            if (IsLocked)
                throw new SynchronizationLockException();

            IsLocked = true;
            return m_Top;
        }

        public bool Pop(Config obj)
        {
            if (!obj.Equals(m_Top))
                return false;

            Prev2T = Prev1T;
            Prev1T = m_Top.Elapsed;
            m_Top = GenerateOne();
            IsLocked = false;
            return true;
        }

        protected virtual Config GenerateEmptyOne()
            => new Config
                   {
                       Token = Guid.NewGuid(),
                       Width = Width++,
                       Strategy = Strategy,
                       Elapsed = 1
                   };

        private Config GenerateOne()
        {
            var config = GenerateEmptyOne();

            if (Prev1T.HasValue &&
                Prev2T.HasValue)
            {
                var ticks = Prev1T.Value * Prev1T.Value / Prev2T.Value;
                config.Elapsed = ticks;
            }
            else if (Prev1T.HasValue)
            {
                var ticks = Prev1T.Value * config.Width;
                config.Elapsed = ticks;
            }

            return config;
        }

        public override string ToString() => $"{(IsLocked ? "LOCKED" : "")} {m_Top} ...";
    }
}
