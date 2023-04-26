using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

public class Email : MonoBehaviour
{

    [SerializeField]
    public TextMeshProUGUI email;
    [SerializeField]
    public TextMeshProUGUI password;
    [SerializeField]
    public TextMeshProUGUI Output;
    
    public void getEmail()
    {
        Output.text = email.text + password.text;

    }



}
