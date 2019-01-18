using UnityEngine;
using System.Collections;

public class GravityAttractor : MonoBehaviour
{
    public float gravity = -9.8f;

    public void Attract(Transform body)
    {
        Vector3 gravityUp = (body.position - transform.position).normalized;
        Vector3 localUp = body.forward;

        // Apply downwards gravity to body
        body.GetComponent<Rigidbody>().AddForce(gravityUp * gravity);
        // Allign bodies up axis with the centre of planet
        body.rotation = Quaternion.FromToRotation(localUp, gravityUp) * body.rotation;
    }
}