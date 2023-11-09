from datetime import datetime
import math
import numpy as np
from scipy.spatial.transform import Rotation

# 입력 파일 이름과 출력 파일 이름 정의
input_filename = "C:/Users/jh813/py_code/test202_GT.txt"
output_filename = "C:/Users/jh813/py_code/test202_GTtf_19.txt"

# 입력 파일 열기
with open(input_filename, "r") as input_file:
    # 출력 파일 열기
    with open(output_filename, "w") as output_file:
        # 각 줄에 대한 처리
        for i, line in enumerate(input_file):
                
            # 줄을 공백을 기준으로 분할
            elements = line.strip().split()
            
            quaternion = np.array([float(elements[4]), float(elements[5]), float(elements[6]), float(elements[7])])
            
            # 원하는 롤 각도 (90도)와 피치 각도 (90도)
            roll_angle_degrees = 90
            pitch_angle_degrees = 90

            # 쿼터니언을 회전 행렬로 변환
            rotation = Rotation.from_quat(quaternion)

            # 롤 방향으로 90도 회전하기 위한 회전 행렬 생성
            roll_rotation = Rotation.from_euler('y', roll_angle_degrees, degrees=True)

            # 피치 방향으로 90도 회전하기 위한 회전 행렬 생성
            pitch_rotation = Rotation.from_euler('x', pitch_angle_degrees, degrees=True)

            # 롤과 피치 회전을 순서대로 적용
            new_rotation = roll_rotation * pitch_rotation * rotation

            # 새로운 쿼터니언 추출
            new_quaternion = new_rotation.as_quat()
            
            # new_line = f"{float(elements[0])} {float(elements[1])} {float(elements[2])} {float(elements[3])} {float(elements[4])} {float(elements[5])} {float(elements[6])} {float(elements[7])}\n"
            
            
            # new_line = f"{float(elements[0])} {float(elements[1])} {float(elements[2])} {float(elements[3])} {float(new_quaternion[0])} {float(new_quaternion[1])} {float(new_quaternion[2])} {float(new_quaternion[3])}\n"
            
            ##웅희 version
            # new_line = f"{float(elements[0])} {-float(elements[3])} {float(elements[2])} {-float(elements[1])} {float(new_quaternion[0])} {float(-new_quaternion[1])} {float(new_quaternion[2])} {float(new_quaternion[3])}\n"
            # new_line = f"{float(elements[0])} {float(elements[1])} {float(elements[2])} {float(elements[3])} {float(new_quaternion[0])} {float(new_quaternion[1])} {float(new_quaternion[2])} {float(new_quaternion[3])}\n"
            new_line = f"{float(elements[0])} {float(elements[3])} {-float(elements[1])} {float(elements[2])} {float(new_quaternion[2])} {float(new_quaternion[1])} {float(new_quaternion[0])} {float(new_quaternion[3])}\n"
            new_line = f"{float(elements[0])} {float(elements[3])} {-float(elements[1])} {float(elements[2])} {float(new_quaternion[2])} {float(new_quaternion[0])} {float(new_quaternion[1])} {float(new_quaternion[3])}\n"
            new_line = f"{float(elements[0])} {float(elements[3])} {-float(elements[1])} {float(elements[2])} {float(elements[6])} {float(elements[5])} {float(elements[4])} {float(elements[7])}\n"
            #######
            # new_line = f"{float(elements[0])} {float(elements[1])} {float(elements[2])} {float(elements[3])} {float(elements[6])} {-float(elements[5])} {-float(elements[4])} {elements[7]}\n"

            
            # new_line = f"{float(elements[0])} {-float(elements[3])} {float(elements[2])} {-float(elements[1])} {-float(elements[4])} {-float(elements[5])} {-float(elements[6])} {elements[7]}\n"
            # 새로운 줄을 출력 파일에 쓰기
            output_file.write(new_line)

print("완료되었습니다.")