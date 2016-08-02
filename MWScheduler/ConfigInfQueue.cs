using System;

namespace MWScheduler
{
    public abstract class ConfigInfQueue : IInfQueue<Config>
    {
        private Config m_Top;

        public Config Top
        {
            get
            {
                if (m_Top == null)
                    Top = GenerateOne();

                return m_Top;
            }

            private set { m_Top = value; }
        }

        public long? Prev1T { get; set; }
        public long? Prev2T { get; set; }
        public int Width { get; set; }
        public string Strategy { get; set; }

        public Config Pop()
        {
            var res = Top;
            Prev2T = Prev1T;
            Prev1T = Top.Elapsed.Ticks;
            Top = GenerateOne();
            return res;
        }

        protected virtual Config GenerateEmptyOne()
            => new Config
                   {
                       Width = Width++,
                       Strategy = Strategy,
                       Elapsed = new TimeSpan(1)
                   };

        private Config GenerateOne()
        {
            var config = GenerateEmptyOne();

            if (Prev1T.HasValue &&
                Prev2T.HasValue)
            {
                var ticks = Prev1T.Value * Prev1T.Value / Prev2T.Value;
                config.Elapsed = new TimeSpan(ticks);
            }
            else if (Prev1T.HasValue)
            {
                var ticks = Prev1T.Value * config.Width;
                config.Elapsed = new TimeSpan(ticks);
            }

            return config;
        }
    }
}
