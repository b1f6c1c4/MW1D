using System;
using System.Diagnostics;

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

    public class Config
    {
        public int Width { get; set; }

        public bool UseTotalMines { get; set; }

        public int TotalMines { get; set; }

        public Rational Probability { get; set; }

        public string Strategy { get; set; }

        public TimeSpan Elapsed { get; set; }

        private string Argument =>
            UseTotalMines
                ? $"{Width} {TotalMines} {Strategy}"
                : $"{Width} {Probability} {Strategy}";

        public string Invoke(string fn)
        {
            var proc =
                new Process
                    {
                        StartInfo =
                            new ProcessStartInfo
                                {
                                    FileName = fn,
                                    Arguments = Argument,
                                    UseShellExecute = false,
                                    RedirectStandardOutput = true,
                                    CreateNoWindow = true
                                }
                    };
            proc.Start();
            var res = proc.StandardOutput.ReadToEnd();
            Elapsed = proc.ExitTime.Subtract(proc.StartTime);
            return res;
        }

        public override string ToString() => $"{Argument} : {Elapsed:g}";

        public string GetFileName() =>
            UseTotalMines
                ? $"{Strategy} 1-{Width}-T{TotalMines}.txt"
                : $"{Strategy} 1-{Width}-P{Probability.ToString().Replace('/', 'o')}.txt";
    }
}
