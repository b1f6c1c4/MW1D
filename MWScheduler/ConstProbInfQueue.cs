namespace MWScheduler
{
    public class ConstProbInfQueue : ConfigInfQueue
    {
        public Rational Probability { get; set; }

        protected override Config GenerateEmptyOne()
        {
            var config = base.GenerateEmptyOne();
            config.UseTotalMines = false;
            config.Probability = Probability;
            return config;
        }
    }
}
