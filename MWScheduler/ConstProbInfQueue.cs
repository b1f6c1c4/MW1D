namespace MWScheduler
{
    public class ConstProbInfQueue : ConfigInfQueue
    {
        public int ProbNumerator { get; set; }

        public int ProbDenominator { get; set; }

        protected override Config GenerateEmptyOne()
        {
            var config = base.GenerateEmptyOne();
            config.UseTotalMines = false;
            config.ProbNumerator = ProbNumerator;
            config.ProbDenominator = ProbDenominator;
            return config;
        }
    }
}
