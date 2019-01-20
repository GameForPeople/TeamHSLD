using UnityEngine;
using System.Collections;

public enum OBJECT
{
    FIXED,
    MOVING
}

[RequireComponent(typeof(Rigidbody))]
public class GravityBody : MonoBehaviour
{
    GravityAttractor planet;
    public OBJECT currentObject;

    void Awake()
    {
        planet = GameObject.FindGameObjectWithTag("Planet").GetComponent<GravityAttractor>();

        // Disable rigidbody gravity and rotation as this is simulated in GravityAttractor script
        GetComponent<Rigidbody>().useGravity = false;
        GetComponent<Rigidbody>().constraints = RigidbodyConstraints.FreezeRotation;
    }

    void FixedUpdate()
    {
        // Allow this body to be influenced by planet's gravity 
        planet.Attract(transform);
    }

    private void OnCollisionEnter(Collision collision)
    {
        if(collision.gameObject.transform.parent.tag.Contains("Planet"))
        {
            if (currentObject.Equals(OBJECT.FIXED))
            {
                Destroy(gameObject.GetComponent<BoxCollider>());
                Destroy(gameObject.GetComponent<GravityBody>());
                Destroy(gameObject.GetComponent<Rigidbody>());
            }
            else
                return;
        }
    }
}