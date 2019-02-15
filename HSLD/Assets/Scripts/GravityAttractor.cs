using UnityEngine;
using System.Collections;

public class GravityAttractor : MonoBehaviour
{
    public float gravity = -9.8f;
    Vector3 gravityUp;
    Vector3 localForward;

    public void Attract(Transform body)
    {
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

        body.GetComponent<Rigidbody>().AddForce(gravityUp * gravity);
        body.rotation = Quaternion.FromToRotation(localForward, gravityUp) * body.rotation;
    }
}