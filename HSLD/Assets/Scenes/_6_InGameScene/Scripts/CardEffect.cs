using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CardEffect : MonoBehaviour {
    public GameObject[] cardObject;
    public GameObject EffectObject;
    public GameObject testMesh;
    Vector2 rotateAmount;
    float speed = 100;

    // Use this for initialization
    void Start () {
        setCard(cardObject[0], testMesh.transform);
    }

    public void setCard(GameObject cardObj, Transform transform)
    {
        Vector3 firstPosition = new Vector3(transform.position.x, transform.position.y, transform.position.z);
        Quaternion firstQuaternion = new Quaternion(transform.rotation.x, transform.rotation.y, transform.rotation.z, transform.rotation.w);
        
        EffectObject = Instantiate(cardObj, firstPosition, firstQuaternion);

        StartCoroutine(movingCard(EffectObject));
    }

    public IEnumerator movingCard(GameObject cardObj)
    {
        while (rotateAmount.x < 365.0f)
        {
            //Debug.Log(rotateAmount.x);
            rotateAmount += new Vector2(Time.deltaTime, 0) * speed;
            cardObj.transform.localRotation = Quaternion.Euler(rotateAmount.y, rotateAmount.x, 0);

            cardObj.transform.localPosition = gameObject.transform.position 
                - (cardObj.transform.rotation * Vector3.forward * 50);

            yield return null;
        }
    }
}
