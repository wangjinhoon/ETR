//using UnityEngine;
//using System.IO;

//public class PointCloudLoader : MonoBehaviour
//{
//    public string filePath = "C:/Users/jh813/rgbd-dataset/apple/apple_1/apple_1_1_190.pcd"; // ����Ʈ Ŭ���� ���� ���
//    public float pointSize = 0.001f; // ���� ũ��

//    private ParticleSystem particleSystem;
//    private ParticleSystem.Particle[] particles;

//    void Start()
//    {
//        particleSystem = GetComponent<ParticleSystem>();
//        print("1");
//        // ���Ͽ��� ������ �б�
//        string[] lines = File.ReadAllLines(filePath);
//        particles = new ParticleSystem.Particle[lines.Length];

//        for (int i = 0; i < lines.Length; i++)
//        {
//            //print(lines[i]);
//            string[] parts = lines[i].Split(' ');

//            // XYZ ��ǥ �б�
//            float x = float.Parse(parts[0]);
//            float y = float.Parse(parts[1]);
//            float z = float.Parse(parts[2]);

//            // ��ƼŬ ����
//            particles[i].position = new Vector3(x, y, z);
//            particles[i].startSize = pointSize;
//            particles[i].startColor = Color.blue;
//        }

//        // ��ƼŬ �ý��ۿ� �� �߰�
//        particleSystem.SetParticles(particles, particles.Length);
//    }
//}
//using System.Collections;
//using System.Collections.Generic;
//using UnityEngine;

//public class Particle : MonoBehaviour
//{
//    public bool playAura = true; //��ƼŬ ���� bool
//    public ParticleSystem particleObject; //��ƼŬ�ý���

//    void Start()
//    {
//        playAura = true;
//        particleObject.Play();
//    }


//    void Update()
//    {
//        if (playAura)
//            particleObject.Play();
//        else if (!playAura)
//            particleObject.Stop();
//    }
//}

//using UnityEngine;
//using System.Collections.Generic;
//using System.IO;
//using System;

//public class PointCloudAggregator : MonoBehaviour
//{
//    public string[] filePaths = Directory.GetFiles("C:/Users/jh813/rgbd-dataset/apple/apple_1","*.pcd"); // ���� PCD ���� ��θ� ������ �迭

//    foreach (string file in filePaths)
//    {
//        Console.WriteLine(file);
//    }

//public float pointSize = 0.001f; // ���� ũ��

//    private ParticleSystem particleSystem;
//    private List<ParticleSystem.Particle> allParticles = new List<ParticleSystem.Particle>();

//    void Start()
//    {
//        particleSystem = GetComponent<ParticleSystem>();

//        foreach (string filePath in filePaths)
//        {
//            LoadPointCloudData(filePath);
//        }

//        // ��� ��ƼŬ�� ��ƼŬ �ý��ۿ� ����
//        particleSystem.SetParticles(allParticles.ToArray(), allParticles.Count);
//    }

//    void LoadPointCloudData(string filePath)
//    {
//        string[] lines = File.ReadAllLines(filePath);

//        for (int i = 0; i < lines.Length; i++)
//        {
//            string[] parts = lines[i].Split(' ');

//            // XYZ ��ǥ �б�
//            float x = float.Parse(parts[0]);
//            float y = float.Parse(parts[1]);
//            float z = float.Parse(parts[2]);
//            float q = float.Parse(parts[3]);
//            byte[] bytes = BitConverter.GetBytes(q);

//            uint intValue = BitConverter.ToUInt32(bytes, 0);

//            byte r = (byte)((intValue >> 16) & 0xFF);
//            byte g = (byte)((intValue >> 8) & 0xFF);
//            byte b = (byte)(intValue & 0xFF);




//            // ��ƼŬ ����
//            ParticleSystem.Particle particle = new ParticleSystem.Particle
//            {
//                position = new Vector3(x, y, z),
//                startSize = pointSize,
//                startColor = new Color32(r, g, b, 255)
//        };

//            allParticles.Add(particle);
//        }
//    }
//}

//using UnityEngine;
//using System.Collections.Generic;
//using System.IO;
//using System;

//public class PointCloudAggregator : MonoBehaviour
//{
//    public string[] filePaths; // ���� PCD ���� ��θ� ������ �迭
//    public float pointSize = 0.001f; // ���� ũ��

//    private ParticleSystem particleSystem;
//    private List<ParticleSystem.Particle> allParticles = new List<ParticleSystem.Particle>();

//    void Start()
//    {
//        // ���� ��θ� �������� �ڵ带 Start �޼��� ���η� �ű�ϴ�.
//        filePaths = Directory.GetFiles("C:/Users/jh813/rgbd-dataset/apple/apple_1", "ex1.pcd");
//        print(filePaths);
//        // ���� ��θ� ����ϴ� �ڵ嵵 Start �޼��� ���η� �ű�ϴ�.
//        foreach (string file in filePaths)
//        {
//            Debug.Log(file); // Unity������ Debug.Log�� ����Ͽ� �α׸� ����մϴ�.
//        }

//        particleSystem = GetComponent<ParticleSystem>();

//        foreach (string filePath in filePaths)
//        {
//            LoadPointCloudData(filePath);
//        }

//        // ��� ��ƼŬ�� ��ƼŬ �ý��ۿ� ����
//        particleSystem.SetParticles(allParticles.ToArray(), allParticles.Count);
//    }

//    void LoadPointCloudData(string filePath)
//    {
//        string[] lines = File.ReadAllLines(filePath);

//        for (int i = 0; i < lines.Length; i++)
//        {
//            if(i < 12){
//                continue;
//            }
//            string[] parts = lines[i].Split(' ');

//            // XYZ ��ǥ �б�
//            float x = float.Parse(parts[0]);
//            float y = float.Parse(parts[1]);
//            float z = float.Parse(parts[2]);
//            float q = float.Parse(parts[3]);
//            byte[] bytes = BitConverter.GetBytes(q);

//            uint intValue = BitConverter.ToUInt32(bytes, 0);

//            byte r = (byte)((intValue >> 16) & 0xFF);
//            byte g = (byte)((intValue >> 8) & 0xFF);
//            byte b = (byte)(intValue & 0xFF);

//            // ��ƼŬ ����
//            ParticleSystem.Particle particle = new ParticleSystem.Particle
//            {
//                position = new Vector3(x, y, z),
//                startSize = pointSize,
//                startColor = new Color32(r, g, b, 255)
//            };

//            allParticles.Add(particle);
//        }
//    }
//}
using UnityEngine;
using System.Collections.Generic;
using System.IO;
using System;

public class PointCloudAggregator : MonoBehaviour
{
    public string[] filePaths; // ���� PLY ���� ��θ� ������ �迭
    public float pointSize = 0.01f; // ���� ũ��

    private ParticleSystem particleSystem;
    private List<ParticleSystem.Particle> allParticles = new List<ParticleSystem.Particle>();

    void Start()
    {
        filePaths = Directory.GetFiles("C:/Users/jh813/rgbd-dataset/apple/apple_1", "*.ply");

        foreach (string file in filePaths)
        {
            Debug.Log(file);
        }

        particleSystem = GetComponent<ParticleSystem>();

        foreach (string filePath in filePaths)
        {
            LoadPointCloudData(filePath);
        }

        // ��� ��ƼŬ�� ��ƼŬ �ý��ۿ� ����
        particleSystem.SetParticles(allParticles.ToArray(), allParticles.Count);
    }

    void LoadPointCloudData(string filePath)
    {
        string[] lines = File.ReadAllLines(filePath);
        bool isDataSection = false;

        foreach (string line in lines)
        {
            if (line.StartsWith("end_header"))
            {
                isDataSection = true;
                continue;
            }

            if (isDataSection)
            {
                string[] parts = line.Split(' ');

                // XYZ ��ǥ �б�
                float x = float.Parse(parts[0]);
                float y = float.Parse(parts[1]);
                float z = float.Parse(parts[2]);

                // RGB ���� �б�
                byte r = byte.Parse(parts[6]);
                byte g = byte.Parse(parts[7]);
                byte b = byte.Parse(parts[8]);

                // ��ƼŬ ����
                ParticleSystem.Particle particle = new ParticleSystem.Particle
                {
                    position = new Vector3(x, y, z),
                    startSize = pointSize,
                    startColor = new Color32(r, g, b, 255)
                };

                allParticles.Add(particle);
            }
        }
    }
}
