using UnityEngine;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System;

public class RobotVisualizer : MonoBehaviour
{
    TcpClient client;
    Thread receiveThread;
    string serverIp = "127.0.0.1";
    int serverPort = 8888;

    Vector3 targetPosition;
    Quaternion targetRotation;
    bool positionUpdated = false;

    void Start()
    {
        client = new TcpClient(serverIp, serverPort);
        receiveThread = new Thread(new ThreadStart(ReceiveData));
        receiveThread.IsBackground = true;
        receiveThread.Start();
    }

    void Update()
    {
        if (positionUpdated)
        {
            transform.position = targetPosition;
            transform.rotation = targetRotation;
            positionUpdated = false;
        }
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
        }
    }

    void ParseData(string data)
    {
        string[] values = data.Split(',');
        Vector3 position = new Vector3(
            -Convert.ToSingle(values[0]) - 0.971244f,
            Convert.ToSingle(values[1]) + 0.82f,
            Convert.ToSingle(values[2]) - 1.34747f);
        Quaternion rotation = Quaternion.Euler(
            float.Parse(values[3]),
            -float.Parse(values[5]),
            float.Parse(values[4]));

        targetPosition = position;
        targetRotation = rotation;
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