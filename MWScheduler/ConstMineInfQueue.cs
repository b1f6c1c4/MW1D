namespace MWScheduler
{
    public class ConstMineInfQueue : ConfigInfQueue
    {
        public bool IncreaseMines { get; set; }

        public int TotalMines { get; set; }

        protected override Config GenerateEmptyOne()
        {
            var config = base.GenerateEmptyOne();
            config.UseTotalMines = true;
            config.TotalMines = TotalMines;
            if (IncreaseMines)
                TotalMines++;
            return config;
        }
    }
}
