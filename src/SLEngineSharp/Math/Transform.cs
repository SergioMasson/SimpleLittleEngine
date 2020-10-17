using System;
using System.Collections.Generic;
using System.Text;

namespace SLEngine
{
    public class Transform
    {
        public Vector3 Position;
        public Vector3 Scale;
        public Quaternion Rotation;

        public Transform()
        {
        }

        public Transform(Quaternion rotate)
        {
            Rotation = rotate;
            Position = new Vector3(0, 0, 0);
            Scale = new Vector3(1, 1, 1);
        }

        public Transform(Vector3 translate)
        {
            Rotation = new Quaternion();
            Position = translate;
            Scale = new Vector3(1, 1, 1);
        }

        public Transform(Quaternion rotate, Vector3 translate)
        {
            Rotation = rotate;
            Position = translate;
            Scale = new Vector3(1, 1, 1);
        }

        public Transform(Quaternion rotate, Vector3 translate, Vector3 scale)
        {
            Rotation = rotate;
            Position = translate;
            Scale = scale;
        }

        public Transform(Matrix mat)
        {
            Rotation = Quaternion.CreateFromRotationMatrix(mat);
            Position = new Vector3(0, 0, 0);
            Scale = new Vector3(1, 1, 1);
        }

        public Transform(Matrix mat, Vector3 translate)
        {
            Rotation = Quaternion.CreateFromRotationMatrix(mat);
            Position = translate;
            Scale = new Vector3(1, 1, 1);
        }

        public static implicit operator Matrix(Transform d)
        {
            Matrix scale = Matrix.CreateScale(d.Scale);
            Matrix rotation = Matrix.CreateFromQuaternion(d.Rotation);
            Matrix translater = Matrix.CreateTranslation(d.Position);

            return scale * rotation * translater;
        }
    }
}