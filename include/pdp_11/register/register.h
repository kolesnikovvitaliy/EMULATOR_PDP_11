#ifndef REGISTER_H
#define REGISTER_H
struct pdp_11_t;
struct register_t;
struct register_t* register_new();

void reg_create(struct pdp_11_t*, struct register_t*);
void reg_destroy(struct pdp_11_t*);



#endif
#pragma once
