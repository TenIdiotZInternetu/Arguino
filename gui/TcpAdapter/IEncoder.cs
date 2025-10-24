namespace TcpAdapter;

public interface IEncoder<T>
{
    public T Decode(string message);
    public string Encode(T data);
}