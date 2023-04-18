using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;
using Firebase;
using Firebase.Database;
using Firebase.Extensions;

public class ReadData : MonoBehaviour
{
    DatabaseReference reference;

    private string userID;
    void Start()
    {
        reference = FirebaseDatabase.DefaultInstance.RootReference;
        userID = SystemInfo.deviceUniqueIdentifier;
        Debug.Log("UserID\n");
        Debug.Log(userID);
    }

    public void Read()
    {
        FirebaseDatabase.DefaultInstance
      .GetReference("temperatura")
      .GetValueAsync().ContinueWithOnMainThread(task => {
          if (task.IsFaulted)
          {
              Debug.Log("nulo");// Handle the error...
          }
          else if (task.IsCompleted)
          {
              DataSnapshot snapshot = task.Result;
              // Do something with snapshot...
          }
      });

    }


}
