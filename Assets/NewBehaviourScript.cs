//using UnityEngine;
//using System.IO;

//public class PointCloudLoader : MonoBehaviour
//{
//    public string filePath = "C:/Users/jh813/rgbd-dataset/apple/apple_1/apple_1_1_190.pcd"; // 포인트 클라우드 파일 경로
//    public float pointSize = 0.001f; // 점의 크기

//    private ParticleSystem particleSystem;
//    private ParticleSystem.Particle[] particles;

//    void Start()
//    {
//        particleSystem = GetComponent<ParticleSystem>();
//        print("1");
//        // 파일에서 데이터 읽기
//        string[] lines = File.ReadAllLines(filePath);
//        particles = new ParticleSystem.Particle[lines.Length];

//        for (int i = 0; i < lines.Length; i++)
//        {
//            //print(lines[i]);
//            string[] parts = lines[i].Split(' ');

//            // XYZ 좌표 읽기
//            float x = float.Parse(parts[0]);
//            float y = float.Parse(parts[1]);
//            float z = float.Parse(parts[2]);

//            // 파티클 설정
//            particles[i].position = new Vector3(x, y, z);
//            particles[i].startSize = pointSize;
//            particles[i].startColor = Color.blue;
//        }

//        // 파티클 시스템에 점 추가
//        particleSystem.SetParticles(particles, particles.Length);
//    }
//}
//using System.Collections;
//using System.Collections.Generic;
//using UnityEngine;

//public class Particle : MonoBehaviour
//{
//    public bool playAura = true; //파티클 제어 bool
//    public ParticleSystem particleObject; //파티클시스템

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
//    public string[] filePaths = Directory.GetFiles("C:/Users/jh813/rgbd-dataset/apple/apple_1","*.pcd"); // 여러 PCD 파일 경로를 저장할 배열

//    foreach (string file in filePaths)
//    {
//        Console.WriteLine(file);
//    }

//public float pointSize = 0.001f; // 점의 크기

//    private ParticleSystem particleSystem;
//    private List<ParticleSystem.Particle> allParticles = new List<ParticleSystem.Particle>();

//    void Start()
//    {
//        particleSystem = GetComponent<ParticleSystem>();

//        foreach (string filePath in filePaths)
//        {
//            LoadPointCloudData(filePath);
//        }

//        // 모든 파티클을 파티클 시스템에 설정
//        particleSystem.SetParticles(allParticles.ToArray(), allParticles.Count);
//    }

//    void LoadPointCloudData(string filePath)
//    {
//        string[] lines = File.ReadAllLines(filePath);

//        for (int i = 0; i < lines.Length; i++)
//        {
//            string[] parts = lines[i].Split(' ');

//            // XYZ 좌표 읽기
//            float x = float.Parse(parts[0]);
//            float y = float.Parse(parts[1]);
//            float z = float.Parse(parts[2]);
//            float q = float.Parse(parts[3]);
//            byte[] bytes = BitConverter.GetBytes(q);

//            uint intValue = BitConverter.ToUInt32(bytes, 0);

//            byte r = (byte)((intValue >> 16) & 0xFF);
//            byte g = (byte)((intValue >> 8) & 0xFF);
//            byte b = (byte)(intValue & 0xFF);




//            // 파티클 설정
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
//    public string[] filePaths; // 여러 PCD 파일 경로를 저장할 배열
//    public float pointSize = 0.001f; // 점의 크기

//    private ParticleSystem particleSystem;
//    private List<ParticleSystem.Particle> allParticles = new List<ParticleSystem.Particle>();

//    void Start()
//    {
//        // 파일 경로를 가져오는 코드를 Start 메서드 내부로 옮깁니다.
//        filePaths = Directory.GetFiles("C:/Users/jh813/rgbd-dataset/apple/apple_1", "ex1.pcd");
//        print(filePaths);
//        // 파일 경로를 출력하는 코드도 Start 메서드 내부로 옮깁니다.
//        foreach (string file in filePaths)
//        {
//            Debug.Log(file); // Unity에서는 Debug.Log를 사용하여 로그를 출력합니다.
//        }

//        particleSystem = GetComponent<ParticleSystem>();

//        foreach (string filePath in filePaths)
//        {
//            LoadPointCloudData(filePath);
//        }

//        // 모든 파티클을 파티클 시스템에 설정
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

//            // XYZ 좌표 읽기
//            float x = float.Parse(parts[0]);
//            float y = float.Parse(parts[1]);
//            float z = float.Parse(parts[2]);
//            float q = float.Parse(parts[3]);
//            byte[] bytes = BitConverter.GetBytes(q);

//            uint intValue = BitConverter.ToUInt32(bytes, 0);

//            byte r = (byte)((intValue >> 16) & 0xFF);
//            byte g = (byte)((intValue >> 8) & 0xFF);
//            byte b = (byte)(intValue & 0xFF);

//            // 파티클 설정
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
    public string[] filePaths; // 여러 PLY 파일 경로를 저장할 배열
    public float pointSize = 0.01f; // 점의 크기

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

        // 모든 파티클을 파티클 시스템에 설정
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

                // XYZ 좌표 읽기
                float x = float.Parse(parts[0]);
                float y = float.Parse(parts[1]);
                float z = float.Parse(parts[2]);

                // RGB 색상 읽기
                byte r = byte.Parse(parts[6]);
                byte g = byte.Parse(parts[7]);
                byte b = byte.Parse(parts[8]);

                // 파티클 설정
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
