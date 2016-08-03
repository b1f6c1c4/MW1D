using System;
using System.Xml.Serialization;

namespace MWScheduler
{
    public class Rational
    {
        // ReSharper disable once UnusedMember.Global
        public Rational() : this(0, 1) { }

        public Rational(int numerator, int denominator)
        {
            Numerator = numerator;
            Denominator = denominator;
        }

        public int Numerator { get; set; }

        public int Denominator { get; set; }

        public override string ToString() => $"{Numerator}/{Denominator}";
    }

    public interface ICacheable
    {
        string FileName { get; }
    }

    public class Config : ICacheable, IEquatable<Config>
    {
        public Config() : this(Guid.NewGuid()) { }

        public Config(Guid token) { Token = token; }

        public Guid Token { get; set; }

        public int Width { get; set; }

        [XmlAttribute]
        public bool UseTotalMines { get; set; }

        public int TotalMines { get; set; }

        public Rational Probability { get; set; }

        public string Strategy { get; set; }

        public long Elapsed { get; set; }

        public string RawOutput { get; set; }

        [XmlIgnore]
        private string Argument =>
            UseTotalMines
                ? $"{Width} {TotalMines} {Strategy}"
                : $"{Width} {Probability} {Strategy}";

        public bool Equals(Config other) => Token == other.Token;

        public override string ToString() => $"{Argument} : {Elapsed:g}";

        [XmlIgnore]
        public string FileName =>
            UseTotalMines
                ? $"{Strategy} 1-{Width}-T{TotalMines}.txt"
                : $"{Strategy} 1-{Width}-P{Probability.ToString().Replace('/', 'o')}.txt";
    }
}
