/*
 * Copyright (C) 2011, 2012 Stian Ellingsen <stian@plaimi.net>
 *
 * This file is part of Limbs Off.
 *
 * Limbs Off is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Limbs Off is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Limbs Off.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "physics.hxx"

void CollisionQueue::add(Collision c) {
    std::list<Collision>::iterator other, end = collisions_.end();
    for (other = collisions_.begin(); other != end; ++other) {
        if (c < *other)
            break;
        if (c.conflicts(*other))
            return;
    }
    collisions_.insert(other, c);
    while (other != end)
        other = c.conflicts(*other) ? collisions_.erase(other) : ++other;
}

bool collideCircleCircle(Circle<phys_t>* ca, Circle<phys_t>* cb, state2p sa,
        state2p sb, state2p& na, state2p& nb, phys_t& t, vector2p& p,
        vector2p& n) {
    phys_t ra = ca->getRadius(), r = ra + cb->getRadius(), rr = r * r;
    vector2p d1 = sb.p - sa.p, d2 = nb.p - na.p;
    phys_t t2, rt;
    if (!intersectLineCircle(d1, d2, rr, t, t2) || t >= 1.0 || t2 < 0.0)
        return false;
    t = max<phys_t> (0.0, t);
    rt = 1 - t;
    na = sa * rt + na * t;
    vector2p pa = na.p;
    nb = sb * rt + nb * t;
    n = (nb.p - pa) / r;
    p = n * ra;
    return true;
}

bool collide(Body* a, Body* b, bodystate& na, bodystate& nb, phys_t& t,
        vector2p& p, vector2p& n) {
    Shape<phys_t> *sa = a->getShape(), *sb = b->getShape();
    ShapeType ta = sa->getType(), tb = sb->getType();
    switch (ta) {
    case CIRCLE:
        switch (tb) {
        case CIRCLE:
            return collideCircleCircle((Circle<phys_t>*) sa,
                    (Circle<phys_t>*) sb, a->getState(), b->getState(), na.l,
                    nb.l, t, p, n);
        default:
            ;
        }
    default:
        ;
    }
    return false;
}

vector2p bounce1(Body* a, Body* b, vector2p& pa, vector2p pb, vector2p n,
        phys_t restitution, phys_t friction, phys_t rr, phys_t addVel) {
    vector2p m = a->getMomentumAt(pa, b->getVelocityAt(pb) - n * addVel);
    vector2p mr = m * (-1 - restitution);
    phys_t rb = n * mr;
    vector2p i = { 0.0, 0.0 };
    if (rb < 0) {
        i = n * rb + ~n * clampmag<phys_t> (~n * mr, -rb * friction);
        phys_t avd = b->getAngularVelocity() - a->getAngularVelocity();
        phys_t am = avd * a->getMomentOfInertia();
        pa += ~n * clampmag<phys_t> ((~pa * i - am) / rb, rr);
    }
    return i;
}

vector2p bounce2(Body* a, Body* b, vector2p& pa, vector2p& pb, vector2p n,
        phys_t restitution, phys_t friction, phys_t rr, phys_t addVel) {
    vector2p va = a->getVelocityAt(pa);
    vector2p vb = b->getVelocityAt(pb);
    vector2p v = va - vb;
    phys_t vn = v * n + addVel;
    // TODO: Impulse for non-circular bodies
    vector2p m = n * (vn / (a->getInvMass() + b->getInvMass()));
    vector2p mr = m * (-1 - restitution);
    phys_t rb = n * mr;
    vector2p i = { 0.0, 0.0 };
    if (rb < 0) {
        i = n * rb + ~n * clampmag<phys_t> (~n * mr, -rb * friction);
        // TODO: Rolling resistance
    }
    return i;
}
