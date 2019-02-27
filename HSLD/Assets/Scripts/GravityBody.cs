using UnityEngine;
using System.Collections;
using System.Collections.Generic;


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
    private float time_ = 0;

    void Start()
    {
        if(GameObject.FindWithTag("InGamePlanet") != null)
            planet = GameObject.FindWithTag("InGamePlanet").GetComponent<GravityAttractor>();
        else
            planet = GameObject.FindWithTag("Planet").GetComponent<GravityAttractor>();

        // Disable rigidbody gravity and rotation as this is simulated in GravityAttractor script
        GetComponent<Rigidbody>().useGravity = false;
        GetComponent<Rigidbody>().constraints = RigidbodyConstraints.FreezeRotation;
    }

    private void FixedUpdate()
    {
        planet.Attract(transform);
        if (currentObject.Equals(OBJECT.FIXED))
        {
            time_ += Time.deltaTime;
            if (time_ > 2.0f)
                Destroy(gameObject);
        }
    }

    private void OnCollisionEnter(Collision collision)
    {
        if (collision.gameObject.transform.parent == null)
            return;

        if (collision.gameObject.transform.parent.tag.Contains("InGamePlanet"))
        {
            if (currentObject.Equals(OBJECT.FIXED))
            {
                Destroy(gameObject.GetComponent<BoxCollider>());
                Destroy(gameObject.GetComponent<GravityBody>());
                Destroy(gameObject.GetComponent<Rigidbody>());
            }
        }
    }
}