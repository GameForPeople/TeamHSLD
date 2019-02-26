using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class UISystem : MonoBehaviour
{
    public void TutorialGo()
    {
        SceneManager.LoadScene("Tutorial");
    }
}
