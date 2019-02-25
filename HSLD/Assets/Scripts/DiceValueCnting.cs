using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class DiceValueCnting : MonoBehaviour
{
    private void OnEnable()
    {
        gameObject.transform.GetChild(0).GetComponent<Text>().text = CameraController.DiceCount.ToString();
    }
}
