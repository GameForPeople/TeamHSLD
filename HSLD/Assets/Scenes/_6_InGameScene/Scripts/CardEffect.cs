using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CardEffect : MonoBehaviour {
    public GameObject[] cardObject;
    public GameObject EffectObject;
    public GameObject testMesh;
    Vector2 rotateAmount;
    float speed = 100;
    float test = 0;

    // Use this for initialization
    void Start () {
        setCard(cardObject[0], testMesh.transform);
    }

    private void Update()
    {
        test++;
        if (test == 1000)
        {
            StartCoroutine(Camera.main.GetComponent<CameraController>().movePositionEffect(testMesh.transform.position));
            StartCoroutine(movingCard(EffectObject));
        }
        
    }

    public void setCard(GameObject cardObj, Transform transform)
    {
        Vector3 firstPosition = new Vector3(transform.position.x, transform.position.y, transform.position.z);
        Quaternion firstQuaternion = new Quaternion(transform.rotation.x, transform.rotation.y, transform.rotation.z, transform.rotation.w);
        
        EffectObject = Instantiate(cardObj, firstPosition, firstQuaternion);

    }

    public IEnumerator movingCard(GameObject cardObj)
    {
        while (rotateAmount.x < 365.0f)
        {
            Vector3 rotate = Vector3.Cross((testMesh.transform.position).normalized, testMesh.transform.right).normalized;
            
            cardObj.transform.RotateAround(gameObject.transform.position, rotate * 120, speed * Time.deltaTime);
            yield return null;
        }
    }
}
