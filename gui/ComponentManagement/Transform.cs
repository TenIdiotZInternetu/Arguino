using System.Numerics;

namespace ComponentManagement;

public record Transform {
    private Vector2 _position = Vector2.Zero;
    private Vector2 _scale = 400 * Vector2.One;         // 100x100 pixels, chosen arbitrarily
    
    public float Rotation { get; set; }

    public Vector2 Position {
        get =>  _position;
        set => _position = value;
    }
    public float PositionX {
        get => _position.X;
        set => _position.X = value;
    }
    public float PositionY {
        get => _position.Y;
        set => _position.Y = value;
    }

    public Vector2 Scale {
        get => _scale;
        set => _scale = value;
    }
    public float ScaleX {
        get => _scale.X;
        set => _scale.X = value;
    }
    public float ScaleY {
        get => _scale.Y;
        set => _scale.Y = value;
    }
}