#include "CameraSimulator.h"


        Mat EventCamera::getDiff_andReconstruction(Mat frame)
        {
            if (theFirstTime == 0)
            {
                Mat BlackScreen_temp(frame.rows, frame.cols, CV_8UC1, Scalar(0, 0, 0));
                Mat in[] = { BlackScreen_temp, BlackScreen_temp, BlackScreen_temp };
                merge(in, 3, frame1_t);
                BlackScreen = frame1_t.clone();
                sum1 = BlackScreen.clone();
                sum2 = BlackScreen.clone();
                theFirstTime++;
            }

            frame2_t = frame1_t.clone();
            frame1_t = frame.clone();

            int** temp_mat_B = new int* [frame1_t.rows];

            for (int i = 0; i < frame1_t.rows; i++) {

                temp_mat_B[i] = new int[frame1_t.cols];
            }
            int** temp_mat_G = new int* [frame1_t.rows];

            for (int i = 0; i < frame1_t.rows; i++) {

                temp_mat_G[i] = new int[frame1_t.cols];
            }
            int** temp_mat_R = new int* [frame1_t.rows];

            for (int i = 0; i < frame1_t.rows; i++) {

                temp_mat_R[i] = new int[frame1_t.cols];
            }

            int ttt1, ttt2, ttt3;


            int threshold_value = 10; //Value of the threshold under which the differences between each frames will be negleted


            for (int i = 0; i < frame1_t.cols; i++)
            {
                for (int j = 0; j < frame1_t.rows; j++)
                {
                    ttt1 = (int)frame1_t.at<Vec3b>(j, i)[0] - (int)frame2_t.at<Vec3b>(j, i)[0];
                    ttt2 = (int)frame1_t.at<Vec3b>(j, i)[1] - (int)frame2_t.at<Vec3b>(j, i)[1];
                    ttt3 = (int)frame1_t.at<Vec3b>(j, i)[2] - (int)frame2_t.at<Vec3b>(j, i)[2];

                    

                    if (abs(ttt1) < threshold_value)
                    {
                        temp_mat_B[j][i] = 0;
                    }
                    else
                    {
                        temp_mat_B[j][i] = ttt1;
                    }

                    if (abs(ttt2) < threshold_value)
                    {
                        temp_mat_G[j][i] = 0;
                    }
                    else
                    {
                        temp_mat_G[j][i] = ttt2;
                    }

                    if (abs(ttt3) < threshold_value)
                    {
                        temp_mat_R[j][i] = 0;
                    }
                    else
                    {
                        temp_mat_R[j][i] = ttt3;
                    }

                }
            }
            vector <int**> mat_BGR{ temp_mat_B ,temp_mat_G ,temp_mat_R };

            sum1 = sum2.clone();

            for (int i = 0; i < sum1.cols; i++)
            {
                for (int j = 0; j < sum1.rows; j++)
                {
                    if ((int)sum1.at<Vec3b>(j, i)[0] + (int)mat_BGR[0][j][i] > 255)
                    {
                        sum2.at<Vec3b>(j, i)[0] = 255;
                    }
                    else if ((int)sum1.at<Vec3b>(j, i)[0] + (int)mat_BGR[0][j][i] < 0)
                    {
                        sum2.at<Vec3b>(j, i)[0] = 0;
                    }
                    else
                    {
                        sum2.at<Vec3b>(j, i)[0] = (int)sum1.at<Vec3b>(j, i)[0] + (int)mat_BGR[0][j][i];
                    }

                    if ((int)sum1.at<Vec3b>(j, i)[1] + (int)mat_BGR[1][j][i] > 255)
                    {
                        sum2.at<Vec3b>(j, i)[1] = 255;
                    }
                    else if ((int)sum1.at<Vec3b>(j, i)[1] + (int)mat_BGR[1][j][i] < 0)
                    {
                        sum2.at<Vec3b>(j, i)[1] = 0;
                    }
                    else
                    {
                        sum2.at<Vec3b>(j, i)[1] = (int)sum1.at<Vec3b>(j, i)[1] + (int)mat_BGR[1][j][i];
                    }

                    if ((int)sum1.at<Vec3b>(j, i)[2] + (int)mat_BGR[2][j][i] > 255)
                    {
                        sum2.at<Vec3b>(j, i)[2] = 255;
                    }
                    else if ((int)sum1.at<Vec3b>(j, i)[2] + (int)mat_BGR[2][j][i] < 0)
                    {
                        sum2.at<Vec3b>(j, i)[2] = 0;
                    }
                    else
                    {
                        sum2.at<Vec3b>(j, i)[2] = (int)sum1.at<Vec3b>(j, i)[2] + (int)mat_BGR[2][j][i];
                    }

                    /*difference.at<Vec3b>(j, i)[0] = (int)mat_BGR[0][j][i];
                    difference.at<Vec3b>(j, i)[1] = (int)mat_BGR[1][j][i];
                    difference.at<Vec3b>(j, i)[2] = (int)mat_BGR[2][j][i];*/
                }
            }
            return sum2.clone();
        }

