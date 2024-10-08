A major revision of the paper \#39 submitted to the CIC Volume 2024/3. In the temporary Appendix C, we detail and emphasize the key differences between the two versions. Additionally, all modifications in the paper are marked in blue for clarity. We thank the reviewers for their valuable comments and list the major parts revised accordingly.

- Reply to Major Comments
    - make a precise statement about the claimed security.

    We have provided the claimed security ofuLBC versions in Section 2.1. uLBC-128/256 are low latency versions, which provide 128/256-bit security in the single-key setting. And the tweakable version uLBC-384t provides 384-bit security in the related-key setting when the tweak length is 0. 
    
    - Explain how you choose $f_1$ and $f_2$.

    In Section 2.4, Line 229-236, we explained how to choose $f_1$ and $f_2$. The main idea is to choose a binary matrix of coefficients and their inverses so that $2$ cancellations can only happen every 15 rounds.

    - Provide implementation results for all versions ofuLBC.

    We list the implementation results for all versions ofuLBC in Table 23.

    - Add differential-linear cryptanalysis.

    We add a new section 4.1.3 to show the main idea and the evaluated results of the differential-linear attack.

    - Explain the details of the boomerang attack.

    We add a new section 4.2 to show how we estimated the upper bound of the number of rounds for the boomerang distinguisher, and we show the longest number of key recovery attacks that can be achieved using the distinguisher.



- Reply to Other Comments
- Review \#39A:
    - the typos
    
    We thank the reviewers for their careful review and pointing out mistakes. We carefully correct most of the errors and strive to improve the quality of our writing.

    - The analysis of delay in section 3.4.1 is to my perspective the best contribution of the paper.  However, it lacks an experimental verification of the model.
    
    Our delay model is adapted from SSH99, a well-established guidebook in circuit design. The model is based on the properties of each CMOS gate, which aligns with current CMOS design principles as outlined in SSH99. This model is proposed to efficiently identify Boolean functions with minimum delay, which has allowed us to derive a lower latency S-box, as detailed in Table 16. While we acknowledge the lack of extensive experimental validation due to the high complexity involved, we believe the underlying logic and principles of this model are robust and provide a reliable foundation for accurately estimating circuit delay.

    - For your largest version uLBC-384t, you claim it can handle 384-bit keys.  Please make a precise statement about the claimed security.  Do you consider related-key attacks up to $2^{384}$ complexity?
    
    In Section 4.6, we show an upper bound estimate of the number of \textsf{uLBC-384t} attack rounds under the relevant key scenario. We show up to 25 attacks can be completed with current techniques, there are still 5 rounds redundancy.


- Review \#39B:
    - Only a very basic type of cryptanalysis is considered. What about differential-linear cryptanalysis, for example?

    We add the results of the differential-linear attack and boomerang attack in sections 4.1.3 and 4.2, respectively. We believe these mainstream attack results effectively illustrate the security of the ciphers, as they provide an upper bound for cryptanalysis. And we have not found any variants that can achieve attacks with the number of rounds beyond the upper bound.
   


- Review \#39C:

    - In Section 3.2, what’s the rationale for choosing the nibble permutation ``PosPerm" to compare the differential diffusion properties of the MDS and almost MDS matrices?

    The choice of the nibble permutation ``PosPerm" in our round function, which uses almost MDS matrices, is crucial because it directly influences the number of rounds required for full diffusion and the number of active S-boxes. These factors are critical in determining the optimal number of rounds for resisting cryptanalysis. While both MDS and almost MDS matrices exhibit at most 8-round differential characteristics, the almost MDS matrices offer a significant advantage in terms of lower latency. This choice effectively balances security with performance efficiency.

    - Can you explain why \textsf{uLBC-128} achieves the smallest area in the hardware evaluation, and why there is a significant area gap compared to QARMA\_9-128?

    The smaller area achieved by \textsf{uLBC-128} in the hardware evaluation compared to QARMA\_9-128 can be attributed to several design efficiencies. Both ciphers employ similar round functions incorporating a 4-bit S-box and linear matrix with 2 XOR gates for each bit. However, QARMA\_9-128 requires 18 rounds (9 rounds for encryption and 9 rounds for decryption) and includes a "Preflector" operation, which involves two S-box operations and a linear matrix, significantly increasing its hardware resource consumption. In contrast, \textsf{uLBC-128} requires only 18 rounds of encryption, and its S-box is specifically designed for better performance in terms of both latency and area compared to QARMA\_9-128.

