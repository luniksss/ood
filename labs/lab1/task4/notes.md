### figures
- circle (`circle <x> <y> <r>`)
- rectangle (`rectangle <left> <top> <width> <height>`)
- triangle (`triangle <x1> <y1> <x2> <y2> <x3> <y3>`)
- line (`line <x1> <y1> <x2> <y2>`)
- text (`text <left> <top> <размер> <текст>`)

### base
1. AddShape  
   `AddShape <id> <цвет> <тип> <параметры>`  
   **_AddShape sh1 #ff00ff circle 100 110 15_**  
   Если фигура с таким id уже существует, выдать сообщение об ошибке.  
2. MoveShape  
   `MoveShape <id> <dx> <dy>`
3. MovePicture  
    `MovePicture <dx> <dy>`
4. Delete Shape  
    `DeleteShape <id>`  
   Если фигуры с таким id не существует, выдать сообщение об ошибке.
5. List  
    `<номер фигуры> <тип> <id> <цвет> <параметры фигуры>`  
    ```
   1 circle circ1 #ff00ff 10.3 20.3 30  
   2 rectangle rect1 #ff00ff 10 20 35 45.5  
   3 triangle tr1 #ff00ff 0.5 23.5 10 25.5 35 11
   4 line ln1 #ff00ff 11.2 15.3 -2.7 2.25
   5 text txt1 #000000 100.3 100.2 12.8 Hello world
   ```
6. ChangeColor  
    `ChangeColor <id> <цвет>`

7. ChangeShape  
    `ChangeShape <id> <тип> <параметры>`
   AddShape sh1 #ff0000 rectangle 10 20 30 40  
   ChangeShape sh1 circle 100 110 15
8. DrawShape  
    `DrawShape <id>`  
   Ранее нарисованное изображение не стирается. То есть эффект команд DrawShape "накапливается"
9. DrawPicture

### diagram
https://mermaid.live/edit#pako:eNrtWXFP4zYU_ypWNKGyK1WvLVACQ5rK2JB6p9MVadKEVHmx21pL4sxxaHuM7z7bcVI7cdJM_HVoRRTH7-dnv_d-fn4OL15AEfZ8Lwhhmt4RuGYweoqB-KgecMfglsTrGY05o2GIGXjJxfLzASw4ZFxjeqcHyRlIOfJ9knKGYXQCoiWJTWmpNopgjH4Tv0K1hKUbmGD9XFMXweRGNYRaMbpfUXMrxgd5M83Hvj7pWetm0LOGVdSGmEI17GEG42eYHoN9IQHPGC70VfxpwS2fzoXXTF9-AD8jtJB-6dlOTaAIVmohP9Fn3BF6h0PMO4PFsjtCZxsYr_GMhpR1Bf-HRWin9mpWF4IjWjST6IGYNOPXpny92vm-DrGUB6plQRRJU9_Xc0pUkjevm3hXY4ey-R4GnLK9TRFTYjNjJtbsDllIYmxZGSjojLAgrGJJmjqgc6GhE_ArDrgIWke1j3jHuwEZadda-lTHBtz8MxiAxf2nubkdS6GQlRuwKlAeLNTFMMJpAgMsI2_6O49GMc4Q6NSHeU5y9Q0C-X1aBUlmPtIeotmfIQa7PtCtfQ0p_d8NKTn1SxiSJMUFPDjgg33ZZIde5lajoqMhIV7xEs9pUrZXImkuyDfcFzbGKQdGBj4BXGhwqjZoUuwWSmIuNc_NmbYE8U35tMFkvakrvCdheCCIpe8ZM7GGj33g6h65u8fGBCWtjA1Ycqol6rkv_o_99xx7nc1Xvv8Vxwiz30mM6Fam861quZB5xGWhIf7WANaEApQxhmOuHqvYTPSNR49Use1IIjkDSLg1T9HWFCtGo6qdnNr0tliucyL4sTiCdHZU7QVnIhOv94ckCXJx3pZjbGDj4ErtU0XIzJ2fTe2aJE5afRxVUu44VBK_A0pT7oCsHhe51-snRuHilxo75OZ5xvJgN8vY26LsTR-Qc0it7FUPWUz-zvAy4exGLf-21CMLyHqKsYvKSmHp2N0ENU2UW-7Md63a9D5Hh9SEHKnJKEtresAJQc6M0zqvWdGBk7yeq2kxy9YGPW3J3ixkncuuO9Nm3S2I8dbqcRpalLmdbDLr4k7O_xVztYQHlPZOG_naNKoc05WwradwnsxqB7BefLTcNUr27ae7ymUvLl91dtIDKkzNLXRAPos8cRSU0-00Z1ZjMOIVbdckidGz7y0NjOhUtC70xAURe29k7pm1F6IlQe6zcMkbz9UjC9CJz87qDfGvJPs3EGGxodvjwSnCeJwQjWFsSj0O6-xT9bu2rk6dH6KlPH7BT-DJC0jAQvzkVfFl6SXqLsyclVeZKBhEJEtb_WlWH-_CmzVHylcHzW5M5Qu-6yapqJqvQav_anXZO6YkK2xtdqe-AbXTUl2LGqX5NanV6WaF-479Le-fb3R1Ku627YeVmqTV2ZWLwrvKEnyfaF9rK5v9rS_j7eJRu3gsO6uQtTCXIDzH8ZpvdHGthp0cXgA4ekfOW7DX99aMIM_nLMN9L8IsgvLRU1ETdm5wJIz0RRNB9pc091WMSWD8B6VRMYzRbL3x_BUMU_GUJUhEXv_zpISoNwozmsXc8z9eXFwoJZ7_4u3E8_B8cD4aXw6Hk_PJcDKdTvre3vNHl4Ph1dXlaDwdT8aT0cXkte99U9MOB-dX0wvxczWcjCajy8vp679-1sLR