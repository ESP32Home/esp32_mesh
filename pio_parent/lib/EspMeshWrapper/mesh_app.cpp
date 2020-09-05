#include "mesh_app.h"

#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "esp_mesh.h"
#include "esp_mesh_internal.h"

static bool is_mesh_connected = false;
static mesh_addr_t mesh_parent_addr;
static int mesh_layer = -1;

void mac_str2tab(mesh_addr_t t,const char* str){

}

void print_mac(uint8_t* add){

}

void print_ip(ip4_addr_t add){

}

esp_err_t esp_mesh_comm_p2p_start(void)
{
    return ESP_OK;
}

void mesh_event_handler(mesh_event_t event)
{
    mesh_addr_t id = {0,};
    static uint8_t last_layer = 0;
    Serial.printf("esp_event_handler:%d", event.id);

    switch (event.id) {
    case MESH_EVENT_STARTED:
        esp_mesh_get_id(&id);
        Serial.printf("<MESH_EVENT_STARTED>ID:");
        print_mac(id.addr);
        is_mesh_connected = false;
        mesh_layer = esp_mesh_get_layer();
        break;
    case MESH_EVENT_STOPPED:
        Serial.printf("<MESH_EVENT_STOPPED>");
        is_mesh_connected = false;
        mesh_layer = esp_mesh_get_layer();
        break;
    case MESH_EVENT_CHILD_CONNECTED:
        Serial.printf("<MESH_EVENT_CHILD_CONNECTED>aid:%d,",event.info.child_connected.aid);
        print_mac(event.info.child_connected.mac);
        break;
    case MESH_EVENT_CHILD_DISCONNECTED:
        Serial.printf("<MESH_EVENT_CHILD_DISCONNECTED>aid:%d, ",event.info.child_disconnected.aid);
        print_mac(event.info.child_disconnected.mac);
        break;
    case MESH_EVENT_ROUTING_TABLE_ADD:
        Serial.printf("<MESH_EVENT_ROUTING_TABLE_ADD>add %d, new:%d",
                 event.info.routing_table.rt_size_change,
                 event.info.routing_table.rt_size_new);
        break;
    case MESH_EVENT_ROUTING_TABLE_REMOVE:
        Serial.printf("<MESH_EVENT_ROUTING_TABLE_REMOVE>remove %d, new:%d",
                 event.info.routing_table.rt_size_change,
                 event.info.routing_table.rt_size_new);
        break;
    case MESH_EVENT_NO_PARENT_FOUND:
        Serial.printf("<MESH_EVENT_NO_PARENT_FOUND>scan times:%d",
                 event.info.no_parent.scan_times);
        /* TODO handler for the failure */
        break;
    case MESH_EVENT_PARENT_CONNECTED:
        esp_mesh_get_id(&id);
        mesh_layer = event.info.connected.self_layer;
        memcpy(&mesh_parent_addr.addr, event.info.connected.connected.bssid, 6);
        Serial.printf("<MESH_EVENT_PARENT_CONNECTED>layer:%d-->%d, parent:%s, ID:",
                 last_layer, mesh_layer, 
                 esp_mesh_is_root() ? "<ROOT>" :
                 (mesh_layer == 2) ? "<layer2>" : "");
                 print_mac(mesh_parent_addr.addr);
                 print_mac(id.addr);
        last_layer = mesh_layer;
        is_mesh_connected = true;
        if (esp_mesh_is_root()) {
            tcpip_adapter_dhcpc_start(TCPIP_ADAPTER_IF_STA);
        }
        esp_mesh_comm_p2p_start();
        break;
    case MESH_EVENT_PARENT_DISCONNECTED:
        Serial.printf("<MESH_EVENT_PARENT_DISCONNECTED>reason:%d",
                 event.info.disconnected.reason);
        is_mesh_connected = false;
        mesh_layer = esp_mesh_get_layer();
        break;
    case MESH_EVENT_LAYER_CHANGE:
        mesh_layer = event.info.layer_change.new_layer;
        Serial.printf("<MESH_EVENT_LAYER_CHANGE>layer:%d-->%d%s",
                 last_layer, mesh_layer,
                 esp_mesh_is_root() ? "<ROOT>" :
                 (mesh_layer == 2) ? "<layer2>" : "");
        last_layer = mesh_layer;
        break;
    case MESH_EVENT_ROOT_ADDRESS:
        Serial.printf("<MESH_EVENT_ROOT_ADDRESS>root address:");
                 print_mac(event.info.root_addr.addr);
        break;
    case MESH_EVENT_ROOT_GOT_IP:
        /* root starts to connect to server */
        Serial.printf("<MESH_EVENT_ROOT_GOT_IP>sta ip:, mask: , gw: ");
                 print_ip(event.info.got_ip.ip_info.ip);
                 print_ip(event.info.got_ip.ip_info.netmask);
                 print_ip(event.info.got_ip.ip_info.gw);
        break;
    case MESH_EVENT_ROOT_LOST_IP:
        Serial.printf("<MESH_EVENT_ROOT_LOST_IP>");
        break;
    case MESH_EVENT_VOTE_STARTED:
        Serial.printf("<MESH_EVENT_VOTE_STARTED>attempts:%d, reason:%d, rc_addr:",
                 event.info.vote_started.attempts,
                 event.info.vote_started.reason);
                 print_mac(event.info.vote_started.rc_addr.addr);
        break;
    case MESH_EVENT_VOTE_STOPPED:
        Serial.printf("<MESH_EVENT_VOTE_STOPPED>");
        break;
    case MESH_EVENT_ROOT_SWITCH_REQ:
        Serial.printf("<MESH_EVENT_ROOT_SWITCH_REQ>reason:%d, rc_addr:",
                 event.info.switch_req.reason);
                 print_mac( event.info.switch_req.rc_addr.addr);
        break;
    case MESH_EVENT_ROOT_SWITCH_ACK:
        /* new root */
        mesh_layer = esp_mesh_get_layer();
        esp_mesh_get_parent_bssid(&mesh_parent_addr);
        Serial.printf("<MESH_EVENT_ROOT_SWITCH_ACK>layer:%d, parent:", mesh_layer);
        print_mac(mesh_parent_addr.addr);
        break;
    case MESH_EVENT_TODS_STATE:
        Serial.printf("<MESH_EVENT_TODS_REACHABLE>state:%d",
                 event.info.toDS_state);
        break;
    case MESH_EVENT_ROOT_FIXED:
        Serial.printf("<MESH_EVENT_ROOT_FIXED>%s",
                 event.info.root_fixed.is_fixed ? "fixed" : "not fixed");
        break;
    case MESH_EVENT_ROOT_ASKED_YIELD:
        Serial.printf("<MESH_EVENT_ROOT_ASKED_YIELD>, rssi:%d, capacity:%d",
                 event.info.root_conflict.rssi,
                 event.info.root_conflict.capacity);
                 print_mac(event.info.root_conflict.addr);
        break;
    case MESH_EVENT_CHANNEL_SWITCH:
        Serial.printf("<MESH_EVENT_CHANNEL_SWITCH>new channel:%d", event.info.channel_switch.channel);
        break;
    case MESH_EVENT_SCAN_DONE:
        Serial.printf("<MESH_EVENT_SCAN_DONE>number:%d",
                 event.info.scan_done.number);
        break;
    case MESH_EVENT_NETWORK_STATE:
        Serial.printf("<MESH_EVENT_NETWORK_STATE>is_rootless:%d",
                 event.info.network_state.is_rootless);
        break;
    case MESH_EVENT_STOP_RECONNECTION:
        Serial.printf("<MESH_EVENT_STOP_RECONNECTION>");
        break;
    case MESH_EVENT_FIND_NETWORK:
        Serial.printf("<MESH_EVENT_FIND_NETWORK>new channel:%d, router BSSID:",event.info.find_network.channel);
        print_mac(event.info.find_network.router_bssid);
        break;
    case MESH_EVENT_ROUTER_SWITCH:
        Serial.printf("<MESH_EVENT_ROUTER_SWITCH>new router:%s, channel:%d, ",
                 event.info.router_switch.ssid, event.info.router_switch.channel);
                 print_mac(event.info.router_switch.bssid);
        break;
    default:
        Serial.printf("unknown id:%d", event.id);
        break;
    }
}


bool MeshApp::start(DynamicJsonDocument &config,DynamicJsonDocument &secret){

    tcpip_adapter_init();
    tcpip_adapter_dhcps_stop(TCPIP_ADAPTER_IF_AP);
    tcpip_adapter_dhcpc_stop(TCPIP_ADAPTER_IF_STA);

    esp_event_loop_init(NULL, NULL);
    wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_config);
    esp_wifi_start();

    esp_mesh_init();
    esp_mesh_set_max_layer(config["mesh"]["max_layer"]);
    esp_mesh_set_vote_percentage(1);
    esp_mesh_set_ap_assoc_expire(10);

    mesh_cfg_t cfg;
    cfg.crypto_funcs = &g_wifi_default_mesh_crypto_funcs;

    mac_str2tab(cfg.mesh_id,config["mesh"]["id"]);

    cfg.event_cb = &mesh_event_handler;
    cfg.channel = config["mesh"]["channel"];
    String router_ssid = secret["wifi"]["access_point"];
    String router_pass = secret["wifi"]["password"];
    String mesh_pass = secret["mesh"]["password"];
    cfg.router.ssid_len = router_ssid.length();
    memcpy((uint8_t *) &cfg.router.ssid, router_ssid.c_str(), cfg.router.ssid_len);
    memcpy((uint8_t *) &cfg.router.password, router_pass.c_str(),router_pass.length());

    // TODO esp_mesh_set_ap_authmode(CONFIG_MESH_AP_AUTHMODE);
    cfg.mesh_ap.max_connection = config["parent"]["ap_connections"];
    memcpy((uint8_t *) &cfg.mesh_ap.password, mesh_pass.c_str(),mesh_pass.length());
    esp_mesh_set_config(&cfg);
    /* mesh start */
    esp_mesh_start();
    Serial.printf("mesh starts successfully, heap:%d, %s\n",  esp_get_free_heap_size(),
             esp_mesh_is_root_fixed() ? "root fixed" : "root not fixed");

    return true;
}
