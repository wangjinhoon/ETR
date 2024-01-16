using UnityEngine;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System;
using System.IO;


public class RobotVisualizer : MonoBehaviour
{
    TcpClient client;
    Thread receiveThread;
    string serverIp = "127.0.0.1";
    int serverPort = 8888;

    Vector3 targetPosition;

    float Roll_;
    float Pitch_;
    float Yaw_;
    bool positionUpdated = false;

    // 카메라 위치 초기화용 데이터 파일 경로
    static string filePath = "C:\\Users\\cam_initi_data.txt";

    // 텍스트 파일 읽어오기
    static string content = ReadFirstLine(filePath);

    // 빈칸을 기준으로 나누어 저장
    static string[] parts = content.Split(' ');


    static string ReadFirstLine(string filePath)
    {
        // 파일이 존재하는지 확인
        if (!File.Exists(filePath))
        {
            throw new FileNotFoundException($"파일을 찾을 수 없습니다: {filePath}");
        }

        // 파일의 첫 번째 줄 읽기
        string firstLine;
        using (StreamReader reader = new StreamReader(filePath))
        {
            firstLine = reader.ReadLine();
        }

        return firstLine;
    }

    void Start()
    {
        //tcp 클라이언트 생성 쓰레드로 계속 ReceiveData함수가 돌아가도록 함
        client = new TcpClient(serverIp, serverPort);
        receiveThread = new Thread(new ThreadStart(ReceiveData));
        receiveThread.IsBackground = true;
        receiveThread.Start();
    }

    void Update()
    {
        if (positionUpdated)
        {
            //카메라 객체의 위치를 현재 targert position과 변환된 롤, 피치, 요로 이동시킴
            transform.position = targetPosition;
            transform.rotation = DeviceWorldRotation_to_UnityWorldRotation_v2(Roll_, Pitch_, Yaw_);

            positionUpdated = false;
        }
    }

    // 디바이스 월드 좌표계에서 유니티 월드 좌표계로 변환
    public Quaternion DeviceWorldRotation_to_UnityWorldRotation_v2(float roll, float pitch, float yaw)
    {

        return
            Quaternion.Euler(0, 0, -roll) *
            Quaternion.Euler(pitch, 0, 0) *
            Quaternion.Euler(0, -yaw, 0);
    }

    void ReceiveData()
    {
        NetworkStream stream = client.GetStream();
        byte[] data = new byte[1024];

        while (true)
        {
            int bytesReceived = stream.Read(data, 0, data.Length);
            string message = Encoding.ASCII.GetString(data, 0, bytesReceived);
            ParseData(message);
            Thread.Sleep(100);
        }
    }
  
    // 받은 데이터를 전처리하여 target 변수에 넣어줌
    void ParseData(string data)
    {
        string[] values = data.Split(',');
        // 카메라 위치 초기화를 위해 받은 데이터에 parts 데이터를 빼준다.
        Vector3 position = new Vector3(
            -Convert.ToSingle(values[0]) - (float.Parse(parts[0])/100),
            Convert.ToSingle(values[1]) - (float.Parse(parts[1])/100),
            Convert.ToSingle(values[2]) - (float.Parse(parts[2])/100));

        Roll_ = Convert.ToSingle(values[3]);
        Pitch_ = Convert.ToSingle(values[4]);
        Yaw_ = Convert.ToSingle(values[5]);


        targetPosition = position;

        positionUpdated = true;
    }

    void OnApplicationQuit()
    {
        if (receiveThread != null)
            receiveThread.Abort();
        if (client != null)
            client.Close();
    }

}