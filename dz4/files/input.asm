; Загрузка первого вектора A в регистры R0-R7
LOAD_CONSTANT R0, #100
LOAD_CONSTANT R1, #101
LOAD_CONSTANT R2, #102
LOAD_CONSTANT R3, #103
LOAD_CONSTANT R4, #104
LOAD_CONSTANT R5, #105
LOAD_CONSTANT R6, #106
LOAD_CONSTANT R7, #107

; Загрузка второго вектора B в регистры R8-R15
LOAD_CONSTANT R8, #200
LOAD_CONSTANT R9, #201
LOAD_CONSTANT R10, #202
LOAD_CONSTANT R11, #203
LOAD_CONSTANT R12, #204
LOAD_CONSTANT R13, #205
LOAD_CONSTANT R14, #206
LOAD_CONSTANT R15, #207

MEMORY_WRITE [R24 + 0], R8
MEMORY_WRITE [R24 + 1], R9
MEMORY_WRITE [R24 + 2], R10
MEMORY_WRITE [R24 + 3], R11
MEMORY_WRITE [R24 + 4], R12
MEMORY_WRITE [R24 + 5], R13
MEMORY_WRITE [R24 + 6], R14
MEMORY_WRITE [R24 + 7], R15
MEMORY_WRITE [R24 + 8], R16

; Поэлементная операция "OR" над элементами векторов A и B, результат сохраняется в векторе C (в регистры R16-R23)
OR R0, [R24 + 0]    ; C[0] = A[0] | B[0]
OR R1, [R24 + 1]    ; C[1] = A[1] | B[1]
OR R2, [R24 + 2]    ; C[2] = A[2] | B[2]
OR R3, [R24 + 3]    ; C[3] = A[3] | B[3]
OR R4, [R24 + 4]    ; C[4] = A[4] | B[4]
OR R5, [R24 + 5]    ; C[5] = A[5] | B[5]
OR R6, [R24 + 6]    ; C[6] = A[6] | B[6]
OR R7, [R24 + 7]    ; C[7] = A[7] | B[7]

; Запись результата вектора C в память (с использованием регистра базы и смещения)
MEMORY_WRITE [R24 + 0], R0
MEMORY_WRITE [R24 + 1], R1
MEMORY_WRITE [R24 + 2], R2
MEMORY_WRITE [R24 + 3], R3
MEMORY_WRITE [R24 + 4], R4
MEMORY_WRITE [R24 + 5], R5
MEMORY_WRITE [R24 + 6], R6
MEMORY_WRITE [R24 + 7], R7