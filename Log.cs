using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using Firebase;
using Firebase.Auth;
using System.Runtime.CompilerServices;

public class Log : MonoBehaviour
{
    [Header("Usuario")]
    public TMP_InputField email;
    public TMP_InputField password;

    [Header("Firebase")]
    public DependencyStatus dependencyStatus;
    public FirebaseAuth auth;
    public FirebaseAuth User;

  /*  public void user_data()
    {
        string data1 = email.text;
        string data2 = password.text;
        Debug.Log(data1 + " " + data2);
    }*/

    private void InitializeFirebase()
    {
        Debug.Log("Configurando Firebase");
        auth = FirebaseAuth.DefaultInstance;
        Debug.Log("Firebase Auth Configurado correctamente");
    }

    public void Login()
    {
        StartCoroutine(Session(email.text, password.text));
    }


    private IEnumerator Session(string _email, string _password)
    {
        var logtask = auth.SignInWithEmailAndPasswordAsync(_email, _password);


        yield return new WaitUntil(predicate: () => logtask.IsCompleted);

        if (logtask.Exception != null)
        {
            Debug.Log(message: $"Fallo en inicio de sesion {logtask.Exception}");
            FirebaseException firebaseEx = logtask.Exception.GetBaseException() as FirebaseException;
            AuthError errorcode = (AuthError)firebaseEx.ErrorCode;

            string message = "Fallo en incio de sesion!";
            switch(errorcode)
            {
                case AuthError.MissingEmail:
                    message = "Falta Email";
                    break;
                case AuthError.MissingPassword:
                    message = "Falta Email";
                    break;

                case AuthError.InvalidEmail:
                    message = "Falta Email";
                    break;

                case AuthError.UserNotFound:
                    message = "Falta Email";
                    break;
            }
            Debug.Log(message);
        }
           
            
        
         Debug.LogFormat("usuario correcto: {0} - {1}", logtask.Result.DisplayName, logtask.Result.UserId);
           
       
     }
}
