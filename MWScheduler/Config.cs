using System;

namespace MWScheduler
{
    public class Rational
    {
        public Rational(int numerator, int denominator)
        {
            Numerator = numerator;
            Denominator = denominator;
        }

        public int Numerator { get; set; }

        public int Denominator { get; set; }

        public override string ToString() => $"{Numerator}/{Denominator}";
    }

    public class LongRational
    {
        public LongRational(long numerator, long denominator)
        {
            Numerator = numerator;
            Denominator = denominator;
        }

        public long Numerator { get; set; }

        public long Denominator { get; set; }

        public override string ToString() => $"{Numerator}/{Denominator}";
    }

    public class Config
    {
        public int Width { get; set; }

        public bool UseTotalMines { get; set; }

        public int TotalMines { get; set; }

        public Rational Probability { get; set; }

        public string Strategy { get; set; }

        public TimeSpan Elapsed { get; set; }

        public LongRational Result { get; set; }

        private string Argument =>
            UseTotalMines
                ? $"{Width} {TotalMines} {Strategy}"
                : $"{Width} {Probability} {Strategy}";

        public override string ToString() => $"{Argument} : {Elapsed:g}";

        public string GetFileName() =>
            UseTotalMines
                ? $"{Strategy} 1-{Width}-T{TotalMines}.txt"
                : $"{Strategy} 1-{Width}-P{Probability.ToString().Replace('/', 'o')}.txt";
    }
}
