using UnityEngine;
using System.Collections;

public class GravityAttractor : MonoBehaviour
{
    public float gravity = -9.8f;

    public void Attract(Transform body)
    {
        Vector3 gravityUp;
        Vector3 localForward;
        if (body.transform.childCount == 0)
        {
            gravityUp = (body.position - transform.position).normalized;
            localForward = body.forward;
        }
        else
        {
            gravityUp = (body.transform.GetChild(0).position - transform.position).normalized;
            localForward = body.transform.GetChild(0).forward;
        }


        // Apply downwards gravity to body
        body.GetComponent<Rigidbody>().AddForce(gravityUp * gravity);
        // Allign bodies up axis with the centre of planet
        if(body.transform.childCount == 0)
            body.rotation = Quaternion.FromToRotation(localForward, gravityUp) * body.rotation;
        else
            body.rotation = Quaternion.FromToRotation(localForward, gravityUp) * body.rotation;
        //body.localEulerAngles = new Vector3(body.localEulerAngles.x, 0, body.localEulerAngles.z);
    }
}