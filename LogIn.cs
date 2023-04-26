using System.Collections;
using System.Collections.Generic;
using TMPro;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.UI;

public class LogIn : MonoBehaviour
{
    public TMP_InputField user;
    public TMP_InputField password;
    public Button login_button;
    // Start is called before the first frame update
    

    public void user_data()
    {
        string data1 = user.text;
        string data2 = password.text;
        Debug.Log(data1 + " " + data2);
    }

 
}
