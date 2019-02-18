using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CardEffect : MonoBehaviour {
    public GameObject[] cardObject;
    public GameObject effectCard;
    public GameObject testMesh;
    Vector2 rotateAmount;
    float speed = 100;

    // Use this for initialization
    void Start () {
        //setCard(cardObject[0], testMesh.transform);
    }

    public void setCard(GameObject cardObj, Transform transform)
    {
        Vector3 firstPosition = new Vector3(testMesh.transform.position.x, testMesh.transform.position.y - 30000, testMesh.transform.position.z);
        Vector3 secondPosition = testMesh.transform.position;

        Quaternion firstQuaternion = new Quaternion(transform.rotation.x, transform.rotation.y, transform.rotation.z, transform.rotation.w);

        effectCard = Instantiate(cardObj, firstPosition, firstQuaternion);

        StartCoroutine(Camera.main.GetComponent<CameraController>().movePositionEffect(testMesh.transform.position));
        StartCoroutine(moveUpCard(effectCard, firstPosition, secondPosition));
    }

    public IEnumerator moveRotateCard(GameObject cardObj)
    {
        while (rotateAmount.x < 365.0f)
        {
            Vector3 rotate = Vector3.Cross((testMesh.transform.position).normalized, testMesh.transform.right).normalized;
            
            cardObj.transform.RotateAround(gameObject.transform.position, rotate * 120, speed * Time.deltaTime);
            yield return null;
        }
    }

    public IEnumerator moveUpCard(GameObject cardObj, Vector3 start, Vector3 end)
    {
        while (effectCard.transform.position.y < end.y -5)
        {
            effectCard.transform.position = Vector3.Lerp(effectCard.transform.position, end, Time.deltaTime);
            yield return null;
        }

        StartCoroutine(moveRotateCard(effectCard));
    }
}
