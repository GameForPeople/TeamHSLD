using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LookatTest : MonoBehaviour
{
    public GameObject target;
    public GameObject obj;

    public bool isLeft;
    public bool isRight;
    public bool isUp;
    public bool isDown;
    public bool isForward;
    public bool isBack;

    void Update ()
    {
        if(isLeft)
        obj.transform.LookAt(target.transform.position, Vector3.left);
        if(isRight)
        obj.transform.LookAt(target.transform.position, Vector3.right);
        if(isUp)
        obj.transform.LookAt(target.transform.position, Vector3.up);
        if(isDown)
        obj.transform.LookAt(target.transform.position, Vector3.down);
        if(isForward)
        obj.transform.LookAt(target.transform.position, Vector3.forward);
        if(isBack)
        obj.transform.LookAt(target.transform.position, Vector3.back);
    }
}
