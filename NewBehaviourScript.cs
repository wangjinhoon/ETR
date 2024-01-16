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

    // ī�޶� ��ġ �ʱ�ȭ�� ������ ���� ���
    static string filePath = "C:\\Users\\cam_initi_data.txt";

    // �ؽ�Ʈ ���� �о����
    static string content = ReadFirstLine(filePath);

    // ��ĭ�� �������� ������ ����
    static string[] parts = content.Split(' ');


    static string ReadFirstLine(string filePath)
    {
        // ������ �����ϴ��� Ȯ��
        if (!File.Exists(filePath))
        {
            throw new FileNotFoundException($"������ ã�� �� �����ϴ�: {filePath}");
        }

        // ������ ù ��° �� �б�
        string firstLine;
        using (StreamReader reader = new StreamReader(filePath))
        {
            firstLine = reader.ReadLine();
        }

        return firstLine;
    }

    void Start()
    {
        //tcp Ŭ���̾�Ʈ ���� ������� ��� ReceiveData�Լ��� ���ư����� ��
        client = new TcpClient(serverIp, serverPort);
        receiveThread = new Thread(new ThreadStart(ReceiveData));
        receiveThread.IsBackground = true;
        receiveThread.Start();
    }

    void Update()
    {
        if (positionUpdated)
        {
            //ī�޶� ��ü�� ��ġ�� ���� targert position�� ��ȯ�� ��, ��ġ, ��� �̵���Ŵ
            transform.position = targetPosition;
            transform.rotation = DeviceWorldRotation_to_UnityWorldRotation_v2(Roll_, Pitch_, Yaw_);

            positionUpdated = false;
        }
    }

    // ����̽� ���� ��ǥ�迡�� ����Ƽ ���� ��ǥ��� ��ȯ
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
  
    // ���� �����͸� ��ó���Ͽ� target ������ �־���
    void ParseData(string data)
    {
        string[] values = data.Split(',');
        // ī�޶� ��ġ �ʱ�ȭ�� ���� ���� �����Ϳ� parts �����͸� ���ش�.
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