namespace MWScheduler
{
    public interface IInfQueue<T>
        where T : class
    {
        T Top { get; }

        T Lock();

        bool Pop(T obj);
    }
}
