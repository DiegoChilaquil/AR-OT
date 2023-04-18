using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Firebase;
using Firebase.Database;
using UnityEngine.UI;
using System.Threading.Tasks;
using Firebase.Extensions;
using TMPro;
using Vuforia;
using Newtonsoft.Json;
using Unity.VisualScripting;
using System.Text.Json;
using System.Linq;

public class DBManager : MonoBehaviour 
{
    //DatabaseReference reference;

    public TMP_Text temp;
    public TMP_Text user;
    public VirtualButtonBehaviour V_on;

    void Start()
    {
        DatabaseReference reference = FirebaseDatabase.DefaultInstance.RootReference;
        //FirebaseDatabase.DefaultInstance.GetReference("temperatura:");
        V_on.RegisterOnButtonPressed(OnButtonPressed_on);
    }

  /*  public void temperatura(ValueChangedEventArgs args)
    {
        DataSnapshot snapshot = args.Snapshot;
        Debug.Log(snapshot.Value);
        temp.text = snapshot.Value.ToString();

    }*/

    public void OnButtonPressed_on(VirtualButtonBehaviour V_on)
    {
        Debug.Log("Getting Temp");
        getTemp();
        Debug.Log("Click");
        //user.text = FirebaseAuth.DefaultInstance.CurrentUser.UserId;
        //Debug.Log(user.text);
    }

    async void getTemp()
    {
        await FirebaseDatabase.DefaultInstance
            .GetReference(@"UsersData/")
            .GetValueAsync().ContinueWithOnMainThread(task =>
            {
                if (task.IsFaulted)
                {
                    Debug.LogError(task);
                    Debug.LogError("No se completo");
                }
                else if (task.IsCompleted)
                {
                    DataSnapshot snapshot = task.Result;
                    Debug.Log("...");
                    // Debug.Log( snapshot.GetRawJsonValue());
                    Debug.Log(snapshot.Child(@"GOcojIibOgOpfGwckT4RNr1zzlE3/lecturas/temperatura").Value.ToString());
                    var t = snapshot.Child(@"GOcojIibOgOpfGwckT4RNr1zzlE3/lecturas/temperatura").Value.ToString();
                    temp.text = t + "°C";
                    Debug.Log("Tarea completada");


                }
            });
    }


    async void gethumidity()
    {
        await FirebaseDatabase.DefaultInstance
            .GetReference(@"UsersData/")
            .GetValueAsync().ContinueWithOnMainThread(task =>
            {
                if (task.IsFaulted)
                {
                    Debug.LogError(task);
                    Debug.LogError("No se completo");
                }
                else if (task.IsCompleted)
                {
                    DataSnapshot snapshot = task.Result;
                    Debug.Log("...");
                    // Debug.Log( snapshot.GetRawJsonValue());
                    Debug.Log(snapshot.Child(@"GOcojIibOgOpfGwckT4RNr1zzlE3/lecturas/temperatura").Value.ToString());
                    var h = snapshot.Child(@"GOcojIibOgOpfGwckT4RNr1zzlE3/lecturas/humedad").Value.ToString();
                    temp.text = "humedad:" + h;
                    Debug.Log("Tarea completada");


                }
            });
    }
}
