#include "spline.h"
#include "ui_spline.h"

spline::spline(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::spline)
{
    ui->setupUi(this);
    connect(ui->draw_widget, SIGNAL(NewPoint()), this, SLOT(PushPoint()));
    connect(ui->draw_widget, SIGNAL(MovePoint(QVector2D)), this, SLOT(UpdatePoint(QVector2D)));
}

spline::~spline()
{
    delete ui;
}

void spline::UpdatePoint(QVector2D new_point)
{
    int N = ui->draw_widget->GetPtNum();
    QVector2D Pn = ui->draw_widget->GetPoints()[N];
    for (int i = 0; i < this->_spline.size(); ++i)
    {
        if(this->_spline[i] == Pn)
        {
            this->_spline[i] = new_point;
            break;
        }
    }
    ui->draw_widget->SetSpline(this->_spline);

    if(ui->Lagrange->checkState() == 2)
    {
        double dt = 0.1*(this->_spline.size()-1)/250;
        QVector<QVector2D> sp_lagrange;
        for(int i = 0; i < 251; ++i)
        {
            QVector2D Pn(this->Lagrange(i*dt).x(), this->Lagrange(i*dt).y());
            sp_lagrange.push_back(Pn);
        }
        ui->draw_widget->SetSplineLagrange(sp_lagrange);
    }

    if(ui->Hermite->checkState() == 2)
    {
        double dt;
        dt = 1.0/250;
        QVector<QVector2D> sp_hermite;
        for(int i = 0; i < 251; ++i)
        {
            QVector2D Pn(this->Hermite(i*dt).x(), this->Hermite(i*dt).y());
            sp_hermite.push_back(Pn);
        }
        ui->draw_widget->SetSplineHermite(sp_hermite);
        if(ui->HermiteDerivative->checkState() == 2)
            ui->draw_widget->SetDerivative(Derivative());
    }

    if(ui->Cubic->checkState() == 2)
    {
        double dt;
        dt = 1.0/250;
        QVector<QVector2D> sp_cubic;
        if(ui->CubicC1->checkState() == 2)
        {
            for(int i = 0; i < 251; ++i)
                sp_cubic.push_back(this->Cubic(i*dt, 1));
        }
        else if(ui->CubicC2->checkState() == 2)
        {
            for(int i = 0; i < 251; ++i)
                sp_cubic.push_back(this->Cubic(i*dt, 2));
        }
        else
            QMessageBox::warning(this, "Warning!", "Select Сondition!");
        ui->draw_widget->SetSplineCubic(sp_cubic);
    }

    if(ui->Bezier->checkState() == 2)
    {
        double dt;
        dt = 1.0/250;
        QVector<QVector2D> cr_bezier;
        for(int i = 0; i < 251; ++i)
        {
            QVector2D Pn = this->Bezier(i*dt);
            cr_bezier.push_back(Pn);
        }
        ui->draw_widget->SetCurvesBezier(cr_bezier);
    }
}

void spline::PushPoint()
{
     QVector2D p = ui->draw_widget->GetPoints().back();
     this->_spline.push_back(p);
     ui->draw_widget->SetSpline(_spline);

     if(ui->Lagrange->checkState() == 2)
     {
         double dt = 0.1*(this->_spline.size()-1)/250;
         QVector<QVector2D> sp_lagrange;
         for(int i = 0; i < 251; ++i)
         {
             QVector2D Pn(this->Lagrange(i*dt).x(), this->Lagrange(i*dt).y());
             sp_lagrange.push_back(Pn);
         }
         ui->draw_widget->SetSplineLagrange(sp_lagrange);
     }

     if(ui->Hermite->checkState() == 2)
     {
         double dt;
         dt = 1.0/250;
         QVector<QVector2D> sp_hermite;
         for(int i = 0; i < 251; ++i)
         {
             QVector2D Pn(this->Hermite(i*dt).x(), this->Hermite(i*dt).y());
             sp_hermite.push_back(Pn);
         }
         ui->draw_widget->SetSplineHermite(sp_hermite);
         if(ui->HermiteDerivative->checkState() == 2)
             ui->draw_widget->SetDerivative(Derivative());
     }

     if(ui->Cubic->checkState() == 2)
     {
         double dt;
         dt = 1.0/250;
         QVector<QVector2D> sp_cubic;
         if(ui->CubicC1->checkState() == 2)
         {
             for(int i = 0; i < 251; ++i)
                 sp_cubic.push_back(this->Cubic(i*dt, 1));
         }
         else if(ui->CubicC2->checkState() == 2)
         {
             for(int i = 0; i < 251; ++i)
                 sp_cubic.push_back(this->Cubic(i*dt, 2));
         }
         else
             QMessageBox::warning(this, "Warning!", "Select Сondition!");
         ui->draw_widget->SetSplineCubic(sp_cubic);
     }

     if(ui->Bezier->checkState() == 2)
     {
         double dt;
         dt = 1.0/250;
         QVector<QVector2D> cr_bezier;
         for(int i = 0; i < 251; ++i)
         {
             QVector2D Pn = this->Bezier(i*dt);
             cr_bezier.push_back(Pn);
         }
         ui->draw_widget->SetCurvesBezier(cr_bezier);
     }
}

QVector2D spline::Lagrange(double t)
{
    QVector2D L(0.0, 0.0);
    double l = 1;
    for(int i = 0; i < this->_spline.size(); ++i)
    {
        l = 1;
        for(int j = 0; j < this->_spline.size(); ++j)
        {
            if(i!=j)
            {
                l *= (t - 0.1*j)/(0.1*i - 0.1*j);
            }
        }
        L += l*this->_spline[i];
    }
    return L;
}

void spline::on_Lagrange_stateChanged(int arg1)
{
    if(arg1 == 0)
    {
        ui->draw_widget->ClearSplineLagrange();
        ui->Lagrange->setCheckState(Qt::Unchecked);
        repaint();
    }
    else if(arg1 == 2)
    {
        if(this->_spline.isEmpty())
        {
            QMessageBox::warning(this, "Warning!", "Add Points!");
            ui->Lagrange->setCheckState(Qt::Unchecked);
        }
        else
        {
            double dt = 0.1*(this->_spline.size()-1)/250;
            QVector<QVector2D> sp_lagrange;
            for(int i = 0; i < 251; ++i)
            {
                QVector2D Pn(this->Lagrange(i*dt).x(), this->Lagrange(i*dt).y());
                sp_lagrange.push_back(Pn);
            }
            ui->draw_widget->SetSplineLagrange(sp_lagrange);
            repaint();
       }
    }
}

QVector<QVector2D> spline::Derivative()
{
    QVector<QVector2D> q;
    double dt = 1.0/(this->_spline.size()-1);
    for(int i = 1; i < this->_spline.size()-1; ++i)
    {
        double t1 = (i-1)*dt;
        double t2 = (i+1)*dt;
        if(this->_spline[i+1].x())
        q.push_back((this->_spline[i+1] - this->_spline[i-1])/(t2-t1)/(this->_spline.size()));
    }
    q.push_front((2*(this->_spline[1] - this->_spline[0]) - q.front())/(this->_spline.size()));
    q.push_back((2*(this->_spline[this->_spline.size()-1] - this->_spline[this->_spline.size()-2]) - q.back())/(this->_spline.size()));
    return q;
}

QVector2D spline::Hermite(double t)
{
    QVector<QVector2D> q = this->Derivative();
    double dt = 1.0/(this->_spline.size()-1);
    double w = 0.0;
    double t1, t2;
    int num = 0;
    for(int i = 0; i < this->_spline.size()-1; ++i)
    {
        if(i*dt <= t  && t <= (i+1)*dt)
        {
            num = i;
            t1 = i*dt;
            t2 = (i+1)*dt;
            w = (t-t1)/(t2-t1);
            break;
        }
    }
    QVector2D r = (2*w*w*w - 3*w*w + 1)*this->_spline[num] + (w*w*w - 2*w*w + w)*q[num] + (-2*w*w*w + 3*w*w)*this->_spline[num+1] + (w*w*w - w*w)*q[num+1];
    return r;
}

void spline::on_Hermite_stateChanged(int arg1)
{
    if(arg1 == 0)
    {
        ui->draw_widget->ClearSplineHermite();
        ui->Hermite->setCheckState(Qt::Unchecked);
        ui->HermiteDerivative->setCheckState(Qt::Unchecked);
        repaint();
    }
    else if(arg1 == 2)
    {
        if(this->_spline.isEmpty())
        {
            QMessageBox::warning(this, "Warning!", "Add Points!");
            ui->Hermite->setCheckState(Qt::Unchecked);
            ui->HermiteDerivative->setCheckState(Qt::Unchecked);
        }
        else
        {
            double dt;
            dt = 1.0/250;
            QVector<QVector2D> sp_hermite;
            for(int i = 0; i < 251; ++i)
            {
                QVector2D Pn = this->Hermite(i*dt);
                sp_hermite.push_back(Pn);
            }
            ui->draw_widget->SetSplineHermite(sp_hermite);
            if(ui->HermiteDerivative->checkState() == 2)
                ui->draw_widget->SetDerivative(Derivative());
            repaint();
        }
    }
}

void spline::on_HermiteDerivative_stateChanged(int arg1)
{
    if(arg1 == 0)
    {
        ui->draw_widget->ClearDerivative();
        ui->HermiteDerivative->setCheckState(Qt::Unchecked);
        repaint();
    }
    else if(arg1 == 2)
    {
        if(this->_spline.isEmpty())
        {
            QMessageBox::warning(this, "Warning!", "Add Points!");
            ui->Hermite->setCheckState(Qt::Unchecked);
            ui->HermiteDerivative->setCheckState(Qt::Unchecked);
        }
        else
        {
            ui->draw_widget->SetDerivative(Derivative());
            repaint();
        }

    }
}

QVector<QVector2D> spline::Gauss(QVector<QVector<double>> S, QVector<QVector2D> P, int condition)
{
    int n = this->_spline.size();
    if(condition == 1)
    {

        //Three-diagonal Gauss
        for(int k = 1; k < n-1; ++k)
        {
            double a = S[k][k];
            P[k] /= a;
            for(int j = k-1; j <= k+1; ++j)
                S[k][j] /= a;

            P[1] -= S[1][0]*P[0];
            S[1][0] -= S[1][0]*S[0][0];
            if(k < n-2)
            {
                double b = S[k+1][k];
                P[k+1] -= b*P[k];
                for(int j = k; j <= k+2; ++j)
                       S[k+1][j] -= b*S[k][j];
            }
        }

        for(int k = n-2; k > 0; --k)
        {
            double a = S[k][k];
            P[k] /= a;
            for(int j = k; j <= k+1; ++j)
                S[k][j] /= a;

            P[n-2] -= S[n-2][n-1]*P[n-1];
            S[n-2][n-1] -= S[n-2][n-1]*S[n-1][n-1];
            if(k > 0)
            {
                double b = S[k-1][k];
                P[k-1] -= b*P[k];
                for(int j = k; j >= k-1; --j)
                    S[k-1][j] -= b*S[k][j];
            }
        }

        //Classic Gauss
        /*for(int k = 0; k < n; ++k)
        {
            double a = S[k][k];
            P[k] /= a;
            for(int j = 0; j < n; ++j)
                S[k][j] /= a;

            for(int i = k+1; i < n; ++i)
            {
                double b = S[i][k];
                P[i] -= b*P[k];
                for(int j = k; j < n; ++j)
                    S[i][j] -= b*S[k][j];
            }
        }

        for(int k = n-1; k >= 0; --k)
        {
            double a = S[k][k];
            P[k] /= a;
            for(int j = 0; j < n; ++j)
                S[k][j] /= a;

            for(int i = k-1; i >= 0; --i)
            {
                double b = S[i][k];
                P[i] -= b*P[k];
                for(int j = k; j >= i; --j)
                    S[i][j] -= b*S[k][j];
            }
        }*/
    }

    else if(condition == 2)
    {
        S[1][1] += S[1][0];
        S[n-2][n-2] += S[n-2][n-1];
        P.pop_front();
        P.pop_back();
        S.pop_front();
        S.pop_back();

        for(int i = 0; i < n-2; ++i)
        {
            S[i].pop_front();
            S[i].pop_back();
        }

        int m = n-2;
        for(int k = 0; k < m; ++k)
        {
            double a = S[k][k];
            P[k] /= a;
            if(k == 0)
            {
                S[0][0] /= a;
                S[0][1] /= a;
            }
            else if(k == m-1)
            {
                S[m-1][m-1] /= a;
                S[m-1][m-2] /= a;
            }
            else
            {
                for(int j = k-1; j <= k+1; ++j)
                    S[k][j] /= a;
            }

            if(k < m-1)
            {
                double b = S[k+1][k];
                P[k+1] -= b*P[k];
                for(int j = k; j <= k+1; ++j)
                       S[k+1][j] -= b*S[k][j];
            }
        }

        for(int k = m-1; k >= 0; --k)
        {
            double a = S[k][k];
            P[k] /= a;
            if(k == m-1)
                S[m-1][m-1] /= a;
            else
            {
                for(int j = k; j <= k+1; ++j)
                    S[k][j] /= a;
            }

            if(k > 0)
            {
                double b = S[k-1][k];
                P[k-1] -= b*P[k];
                S[k-1][k] -= b*S[k][k];
            }
        }

        P.push_front(P.front());
        P.push_back(P.back());
    }

    return P;
}

QVector2D spline::Cubic(double t, int condition)
{
    double dt = 1.0/(this->_spline.size()-1);
    double t1, t2;
    int num = 0;
    for(int i = 0; i < this->_spline.size()-1; ++i)
    {
        if(i*dt <= t  && t <= (i+1)*dt)
        {
            num = i;
            t1 = i*dt;
            t2 = (i+1)*dt;
            break;
        }
    }

    QVector<QVector<double>> S;
    QVector<QVector2D> P(this->_spline.size());
    for(int i = 1; i < this->_spline.size()-1; ++i)
    {
        QVector<double> si(this->_spline.size(), 0.0);
        si[i-1] = i*dt - (i-1)*dt;
        si[i] = 2*((i+1)*dt - (i-1)*dt);
        si[i+1] = (i+1)*dt - i*dt;
        S.push_back(si);
        P[i] = 6*((this->_spline[i+1] - this->_spline[i])/((i+1)*dt - i*dt) - (this->_spline[i] - this->_spline[i-1])/(i*dt - (i-1)*dt));
    }

    QVector2D r;
    QVector<double> si(this->_spline.size(), 0.0);
    si[0] = 1.0;
    S.push_front(si);
    P[0] = {0.0, 0.0};
    si[0] = 0.0;
    si[this->_spline.size()-1] = 1.0;
    S.push_back(si);
    P[this->_spline.size()-1] = {0.0, 0.0};

    QVector<QVector2D> s = Gauss(S, P, condition);
    r = s[num]/6*pow(t2-t, 3)/(t2-t1) + s[num+1]/6*pow(t-t1, 3)/(t2-t1) + (this->_spline[num]/(t2-t1) - s[num]*(t2-t1)/6)*(t2-t) + (this->_spline[num+1]/(t2-t1) - s[num+1]*(t2-t1)/6)*(t-t1);
    //r = this->_spline[num+1] + (t - t2)*((this->_spline[num+1]-this->_spline[num])/(t2-t1) + s[num+1]*(t2-t1)/3 + s[num]*(t2-t1)/6) + s[num+1]*pow(t-t2, 2)/2 + pow(t-t2, 3)*((s[num+1] - s[num])/(t2-t1))/6;
    return r;
}

void spline::on_Cubic_stateChanged(int arg1)
{
    if(arg1 == 0)
    {
        ui->draw_widget->ClearSplineCubic();
        ui->Cubic->setCheckState(Qt::Unchecked);
        ui->CubicC1->setCheckState(Qt::Unchecked);
        ui->CubicC2->setCheckState(Qt::Unchecked);
        repaint();
    }
    else if(arg1 == 2)
    {
        if(this->_spline.isEmpty())
        {
            QMessageBox::warning(this, "Warning!", "Add Points!");
            ui->Cubic->setCheckState(Qt::Unchecked);
            ui->CubicC1->setCheckState(Qt::Unchecked);
            ui->CubicC2->setCheckState(Qt::Unchecked);
        }
        else
        {
            double dt;
            dt = 1.0/250;
            QVector<QVector2D> sp_cubic;
            if(ui->CubicC1->checkState() == 2)
            {
                for(int i = 0; i < 251; ++i)
                    sp_cubic.push_back(this->Cubic(i*dt, 1));
            }
            else if(ui->CubicC2->checkState() == 2)
            {
                for(int i = 0; i < 251; ++i)
                    sp_cubic.push_back(this->Cubic(i*dt, 2));
            }
            else
            {
                QMessageBox::warning(this, "Warning!", "Select Сondition!");
                ui->Cubic->setCheckState(Qt::Unchecked);
            }
            ui->draw_widget->SetSplineCubic(sp_cubic);
            repaint();
        }
    }
}

void spline::on_CubicC1_stateChanged(int arg1)
{
    if(arg1 == 0)
    {
        ui->draw_widget->ClearSplineCubic();
        if(ui->CubicC2->checkState() == 0)
            ui->Cubic->setCheckState(Qt::Unchecked);
        repaint();
    }
    else if(arg1 == 2)
    {
        if(ui->CubicC2->checkState() == 2)
        {
            ui->CubicC2->setCheckState(Qt::Unchecked);
            if(ui->Cubic->checkState() == 2)
            {
                double dt;
                dt = 1.0/250;
                QVector<QVector2D> sp_cubic;
                for(int i = 0; i < 251; ++i)
                    sp_cubic.push_back(this->Cubic(i*dt, 1));
                ui->draw_widget->SetSplineCubic(sp_cubic);
                repaint();
            }
        }
    }
}

void spline::on_CubicC2_stateChanged(int arg1)
{
    if(arg1 == 0)
    {
        ui->draw_widget->ClearSplineCubic();
        if(ui->CubicC1->checkState() == 0)
            ui->Cubic->setCheckState(Qt::Unchecked);
        repaint();
    }
    else if(arg1 == 2)
    {
        if(ui->CubicC1->checkState() == 2)
        {
            ui->CubicC1->setCheckState(Qt::Unchecked);
            if(ui->Cubic->checkState() == 2)
            {
                double dt;
                dt = 1.0/250;
                QVector<QVector2D> sp_cubic;
                for(int i = 0; i < 251; ++i)
                    sp_cubic.push_back(this->Cubic(i*dt, 2));
                ui->draw_widget->SetSplineCubic(sp_cubic);
                repaint();
            }
        }
    }
}


double spline::fact(int n)
{
    if(n == 0)
        return 1;
    else if(n == 1)
        return 1;
    else
        return n*fact(n-1);
}

double spline::Cnk(int n, int k)
{
   return (fact(n))/(fact(k)*fact(n-k));
}

QVector2D spline::Bezier(double t)
{
    QVector2D B(0.0, 0.0);
    int n = this->_spline.size()-1;
    for(int i = 0; i <= n; ++i)
        B += this->_spline[i]*Cnk(n, i)*pow(t, i)*pow((1-t), (n-i));
    return B;
}

void spline::on_Bezier_stateChanged(int arg1)
{
    if(arg1 == 0)
    {
        ui->draw_widget->ClearCurvesBezier();
        ui->Bezier->setCheckState(Qt::Unchecked);
        repaint();
    }
    else if(arg1 == 2)
    {
        if(this->_spline.isEmpty())
        {
            QMessageBox::warning(this, "Warning!", "Add Points!");
            ui->Bezier->setCheckState(Qt::Unchecked);
        }
        else
        {
            double dt;
            dt = 1.0/250;
            QVector<QVector2D> cr_bezier;
            for(int i = 0; i < 251; ++i)
            {
                QVector2D Pn = this->Bezier(i*dt);
                cr_bezier.push_back(Pn);
            }
            ui->draw_widget->SetCurvesBezier(cr_bezier);
            repaint();
        }
    }
}

void spline::on_Reset_clicked()
{
    ui->draw_widget->Reset();
    this->_spline.clear();
    ui->Lagrange->setCheckState(Qt::Unchecked);
    ui->Hermite->setCheckState(Qt::Unchecked);
    ui->HermiteDerivative->setCheckState(Qt::Unchecked);
    ui->Cubic->setCheckState(Qt::Unchecked);
    ui->CubicC1->setCheckState(Qt::Unchecked);
    ui->CubicC2->setCheckState(Qt::Unchecked);
    ui->Bezier->setCheckState(Qt::Unchecked);
}

